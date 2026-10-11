// ЛР1. Абстрактные структуры данных (Go). Вариант 3: Complete Binary Tree.
// Запуск: ./dbms --file file.data --query 'SPUSH mystack item'
// Строки с пробелами - в двойных кавычках: --query 'MPUSH arr "hello world"'
// Файл: по строке на контейнер "<тип> <имя> <элементы>"; типы: M F L S Q T.
package main

import (
	"fmt"
	"os"
	"strings"
)

func main() {
	file := "file.data"
	query := ""
	hasQuery := false
	for i := 1; i+1 < len(os.Args); i = i + 2 {
		switch os.Args[i] {
		case "--file":
			file = os.Args[i+1]
		case "--query":
			query = os.Args[i+1]
			hasQuery = true
		}
	}
	if !hasQuery {
		fmt.Fprintf(os.Stderr, "usage: %s --file file.data --query 'COMMAND name args'\n", os.Args[0])
		os.Exit(2)
	}

	defer func() { // перехватываем ошибки, брошенные через fail
		if r := recover(); r != nil {
			e, ok := r.(DbError)
			if !ok {
				panic(r) // это не наша ошибка, а настоящая авария
			}
			fmt.Fprintln(os.Stderr, "ERROR:", string(e))
			os.Exit(1)
		}
	}()

	db := &Database{items: map[string]Container{}}
	db.Load(file)
	a := tokenize(query)
	if len(a) == 0 {
		fail("empty query")
	}
	fmt.Println(execute(db, a))
	if !readOnly(strings.ToUpper(a[0])) { // запись в файл только при изменении
		db.Save(file)
	}
}
