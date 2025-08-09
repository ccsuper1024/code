package main

import (
	"fmt"
	"os"
)

func main() {
	var s, sep string
	for i := 1; i < len(os.Args); i++ {
		//利用了os.Args是个字符串切片--简单的动态数组
		s += sep + os.Args[i]
		sep += " "
	}
	fmt.Println(s)
}
