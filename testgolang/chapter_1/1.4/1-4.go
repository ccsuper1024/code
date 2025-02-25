//修改dup2 ,出现重复行时打印文件名称
package main

import (
	"bufio"
	"fmt"
	"os"
)

func main() {
	//counts的类型为map[string]int
	//make创建一个空map
	counts := make(map[string]int)
	//files的类型为: []string
	files := os.Args[1:]
	if len(files) == 0 {
		countLines(os.Stdin, counts)
	} else {
		//range函数的for循环形式
		for _, arg := range files {
			f, err := os.Open(arg)
			if err != nil {
				fmt.Fprintf(os.Stderr, "dup2: %v\n", err)
				continue
			}
			//counts是map数据结构的引用。在函数内修改会直接修改原始数据本身
			countLines(f, counts)
			f.Close()
		}
	}
	//返回键和对应的值
	for line, n := range counts {
		if n >= 1 {
			fmt.Printf("%d\t%s", n, line)
			if n >= 2 {
				fmt.Printf("\t%s\n", os.Args[0])
			} else {
				fmt.Printf("\n")
			}
		}
	}

}

func countLines(f *os.File, counts map[string]int) {
	//func bufio.NewScanner(r io.Reader) *bufio.Scanner
	input := bufio.NewScanner(f)
	for input.Scan() {
		counts[input.Text()]++
	}

}
