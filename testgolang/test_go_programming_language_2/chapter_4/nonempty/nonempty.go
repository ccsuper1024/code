package main

import "fmt"

func nonempty(stringVec []string) []string {
	i := 0
	for _, s := range stringVec {
		if s != "" {
			stringVec[i] = s
			i++
		}
	}
	return stringVec[:i]
}
func nonempty2(stringVec []string) []string {
	out := stringVec[:0] //zero-length slice of original
	for _, s := range stringVec {
		if s != "" {
			out = append(out, s)
		}
	}
	return out
}

type Stack []int

func StackPush(s Stack, value int) Stack {
	return append(s, value)
}
func StackPop(s Stack) Stack {
	return s[:len(s)-1]
}
func Top(s Stack) int {
	return s[len(s)-1]
}
func Remove(s Stack, i int) []int {
	copy(s[i:], s[i+1:])
	return s[:len(s)-1]
}
func main() {
	data := []string{"one", "", "three"}
	// fmt.Printf("%q\n", nonempty(data))
	// fmt.Printf("%q\n", data)
	// fmt.Printf("%q\n", nonempty2(data))
	data = nonempty2(data)
	fmt.Printf("%q\n", data)
}
