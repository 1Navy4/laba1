// "База данных": набор именованных структур + загрузка/сохранение в файл.
package main

import (
	"os"
	"sort"
	"strings"
)

func makeContainer(t byte) Container {
	switch t {
	case 'M':
		return NewArray()
	case 'F':
		return NewSList()
	case 'L':
		return NewDList()
	case 'S':
		return NewStack()
	case 'Q':
		return NewQueue()
	case 'T':
		return NewCBTree()
	}
	fail("unknown container type in file: " + string(rune(t)))
	return nil
}

type Database struct{ items map[string]Container }

func (db *Database) Load(path string) {
	data, err := os.ReadFile(path)
	if err != nil { // нет файла - база пустая
		return
	}
	for _, line := range strings.Split(string(data), "\n") {
		f := tokenize(line)
		if len(f) < 2 || f[0] == "" {
			continue
		}
		c := makeContainer(f[0][0])
		c.Load(f[2:])
		db.items[f[1]] = c
	}
}

func (db *Database) Save(path string) {
	names := make([]string, 0, len(db.items))
	for name := range db.items {
		names = append(names, name)
	}
	sort.Strings(names) // имена по алфавиту, как в std::map у C++ версии
	var sb strings.Builder
	for _, name := range names {
		c := db.items[name]
		sb.WriteString(string(rune(c.Type())) + " " + quote(name))
		for _, s := range c.Dump() {
			sb.WriteString(" " + quote(s)) // строки с пробелами идут в кавычках
		}
		sb.WriteString("\n")
	}
	if err := os.WriteFile(path, []byte(sb.String()), 0644); err != nil {
		fail("cannot write file: " + path)
	}
}

// fetch находит контейнер типа T; при create=true создает, если его нет.
func fetch[T Container](db *Database, name string, create bool, mk func() T) T {
	c, ok := db.items[name]
	if !ok {
		if !create {
			fail("no such container: " + name)
		}
		t := mk()
		db.items[name] = t
		return t
	}
	t, ok := c.(T)
	if !ok {
		fail("container '" + name + "' has a different type")
	}
	return t
}
