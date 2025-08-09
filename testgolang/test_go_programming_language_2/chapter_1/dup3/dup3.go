package main

import (
	"fmt"
	"os"
	"strings"
)

func main() {
	counts := make(map[string]int)
	for _, filename := range os.Args[1:] {
		/*io.ReadAll会一次性读取文件中所有的数据到内存中*/
		data, err := os.ReadFile(filename)
		if err != nil {
			fmt.Fprintf(os.Stderr, "dup3:%v\n", err)
			continue
		}

		/*用换行符作为数据的分隔符*/
		for _, line := range strings.Split(string(data), "\n") {
			counts[line]++
		}
	}
	fmt.Println("重复的行:")
	for line, n := range counts {
		if n > 1 {
			fmt.Printf("%d\t%s\n", n, line)
		}
	}
}
