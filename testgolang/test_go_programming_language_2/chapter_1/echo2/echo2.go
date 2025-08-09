package main

import (
	"fmt"
	"os"
)

func main() {
	s, sep := "", "" //短变量声明并定义
	for _, arg := range os.Args[1:] {
		s += sep + arg
		sep = " "
	}
	fmt.Println(s)

	a := "1312"
	fmt.Println(a)

	var b string
	b = "2323"
	fmt.Println(b)

	var c = "5464"
	fmt.Println(c)

	var d string = "7894"
	fmt.Println(d)

}
