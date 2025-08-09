package main

import (
	"fmt"
	"strings"
)

func basename(s string) string {
	slash := strings.LastIndex(s, "/") //-1 if "/" not found
	if slash >= 0 {
		s = s[slash+1:]
	}
	fmt.Printf("s = %s\n", s)
	if dot := strings.LastIndex(s, "."); dot >= 0 {
		s = s[:dot]
		fmt.Printf("s = %s\n", s)
	}
	return s
}

func main() {
	fmt.Println(basename("a/b/c.go"))
	fmt.Println(basename("c.d.go"))
	fmt.Println(basename("abc"))
}
