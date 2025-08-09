package main

import (
	"fmt"
	"os"
)

func comma(s string) string {
	n := len(s)
	if n < 3 {
		return s
	}
	return comma(s[:n-3]) + "," + s[n-3:]
}

func main() {
	if len(os.Args) != 2 {
		fmt.Printf("Usage: ./exe number\n")
		os.Exit(1)
	}
	fmt.Printf("os.Args[0] = %s\n", os.Args[0])
	fmt.Println(comma(os.Args[1]))
}
