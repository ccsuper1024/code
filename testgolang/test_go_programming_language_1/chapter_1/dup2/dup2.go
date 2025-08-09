//通过具名文件来读取数据，并统计行数
//Dup2 prints the count of text of lines that appear more than once
//in the input. It reads from stdin or from a list of named files.
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
			fmt.Printf("%d\t%s\n", n, line)
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
