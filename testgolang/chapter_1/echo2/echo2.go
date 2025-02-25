//Echo2 prints its command-line argument
package main

import (
	"fmt"
	"os"
)

func main() {
	s, sep := "", "" //简短变量声明的变量的类型由编译期自动推导
	for _, arg := range os.Args[1:] {
		s += sep + arg
		sep = " "
	}
	fmt.Println(s)
}
