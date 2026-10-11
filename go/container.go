// Общие интерфейсы: Container (все структуры) и List (оба списка).
package main

// Общий интерфейс всех структур: запись в файл (Dump), чтение (Load), вывод (Show).
type Container interface {
	Type() byte
	Dump() []string
	Load(tokens []string)
	Show() string
}

// Оба списка (односвязный и двусвязный) имеют одинаковые операции,
// поэтому команды F... и L... обрабатываются одной функцией listCommand.

type List interface {
	PushHead(v string)
	PushTail(v string)
	PushBefore(target, v string)
	PushAfter(target, v string)
	PopHead() string
	PopTail() string
	DelBefore(target string) string
	DelAfter(target string) string
	DelValue(v string) bool
	Find(v string) int
	At(i int) string
	Items() []string
}
