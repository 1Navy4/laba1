// Стек (S) строк на основе односвязного списка (вершина - голова списка).
package main

type Stack struct{ l SList }

func NewStack() *Stack { return &Stack{} }

func (s *Stack) Push(v string) { s.l.PushHead(v) } // O(1)

func (s *Stack) Pop() string { // O(1)
	if s.l.n == 0 {
		fail("stack is empty")
	}
	return s.l.PopHead()
}

func (s *Stack) Items() []string { return s.l.Items() } // от вершины к низу

func (s *Stack) Type() byte     { return 'S' }
func (s *Stack) Dump() []string { return reversed(s.l.Items()) } // от низа к вершине
func (s *Stack) Load(t []string) {
	for _, v := range t {
		s.Push(v)
	}
}
func (s *Stack) Show() string {
	v := s.l.Items()
	if len(v) == 0 {
		return "(empty stack)"
	}
	r := ""
	for i, x := range v {
		r = r + "| " + x + " |"
		if i == 0 {
			r = r + "  <- top"
		}
		r = r + "\n"
	}
	return r + "+---+"
}
