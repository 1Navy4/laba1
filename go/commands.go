// Разбор и выполнение команд (MPUSH, FDEL, SPOP, TINSERT, PRINT ...).
package main

import (
	"strconv"
	"strings"
)

func listCommand(l List, op string, a []string) string {
	switch op {
	case "PUSH", "PUSHTAIL":
		l.PushTail(arg(a, 2))
		return a[2]
	case "PUSHHEAD":
		l.PushHead(arg(a, 2))
		return a[2]
	case "PUSHBEFORE": // имя контейнера, опорный элемент, новое значение
		l.PushBefore(arg(a, 2), arg(a, 3))
		return a[3]
	case "PUSHAFTER":
		l.PushAfter(arg(a, 2), arg(a, 3))
		return a[3]
	case "DEL": // удаление по значению
		if !l.DelValue(arg(a, 2)) {
			fail("value not found: " + a[2])
		}
		return a[2]
	case "DELHEAD":
		return l.PopHead()
	case "DELTAIL":
		return l.PopTail()
	case "DELBEFORE":
		return l.DelBefore(arg(a, 2))
	case "DELAFTER":
		return l.DelAfter(arg(a, 2))
	case "GET":
		if len(a) > 2 {
			return l.At(toIndex(a[2]))
		}
		return spaced(l.Items())
	case "FIND":
		i := l.Find(arg(a, 2))
		if i < 0 {
			return "NOT FOUND"
		}
		return "FOUND at index " + strconv.Itoa(i)
	}
	fail("unknown command: " + a[0])
	return ""
}

func execute(db *Database, a []string) string {
	cmd := strings.ToUpper(a[0])
	name := arg(a, 1)
	if cmd == "PRINT" {
		c, ok := db.items[name]
		if !ok {
			fail("no such container: " + name)
		}
		return c.Show()
	}
	if cmd == "ISMEMBER" {
		if fetch(db, name, false, NewCBTree).Contains(arg(a, 2)) {
			return "TRUE"
		}
		return "FALSE"
	}

	create := strings.Contains(cmd, "PUSH") || cmd == "TINSERT"
	switch cmd[0] {
	case 'M':
		m := fetch(db, name, create, NewArray)
		switch cmd {
		case "MPUSH": // MPUSH name value [index]
			if len(a) > 3 {
				m.Insert(toIndex(a[3]), arg(a, 2))
			} else {
				m.Push(arg(a, 2))
			}
			return a[2]
		case "MGET": // MGET name [index]
			if len(a) > 2 {
				return m.Get(toIndex(a[2]))
			}
			return spaced(m.Dump())
		case "MSET": // MSET name index value
			m.Set(toIndex(arg(a, 2)), arg(a, 3))
			return a[3]
		case "MDEL": // MDEL name index
			return m.Erase(toIndex(arg(a, 2)))
		case "MLEN":
			return strconv.Itoa(m.Size())
		}
	case 'F':
		return listCommand(fetch(db, name, create, NewSList), cmd[1:], a)
	case 'L':
		l := fetch(db, name, create, NewDList)
		if cmd == "LGETREV" { // чтение с конца - только у двусвязного
			return spaced(l.ItemsReversed())
		}
		return listCommand(l, cmd[1:], a)
	case 'S':
		s := fetch(db, name, create, NewStack)
		switch cmd {
		case "SPUSH":
			s.Push(arg(a, 2))
			return a[2]
		case "SPOP":
			return s.Pop()
		case "SGET": // чтение без удаления: от вершины к низу
			return spaced(s.Items())
		}
	case 'Q':
		q := fetch(db, name, create, NewQueue)
		switch cmd {
		case "QPUSH":
			q.Push(arg(a, 2))
			return a[2]
		case "QPOP":
			return q.Pop()
		case "QGET": // чтение без удаления: от начала к концу
			return spaced(q.Items())
		}
	case 'T':
		t := fetch(db, name, create, NewCBTree)
		switch cmd {
		case "TINSERT":
			return t.Insert(arg(a, 2))
		case "TGET": // TGET name [inorder|preorder|postorder|bfs], по умолчанию bfs
			order := "BFS"
			if len(a) > 2 {
				order = strings.ToUpper(a[2])
			}
			return spaced(t.Traverse(order))
		case "TISCOMPLETE": // вместо удаления
			if t.IsComplete() {
				return "TRUE"
			}
			return "FALSE"
		}
	}
	fail("unknown command: " + a[0])
	return ""
}

func readOnly(cmd string) bool { // эти команды файл не меняют
	return cmd == "PRINT" || cmd == "ISMEMBER" ||
		strings.HasSuffix(cmd, "GET") || strings.HasSuffix(cmd, "FIND") ||
		strings.HasSuffix(cmd, "LEN") || strings.HasSuffix(cmd, "COMPLETE") ||
		strings.HasSuffix(cmd, "GETREV")
}
