/*
Dup2 prints the count and text of lines that appear more than once
int the input. It reads from stdin or from a list of named files
*/
package main

import (
	"bufio"
	"fmt"
	"os"
)

func countLines(f *os.File, counts map[string]int) {
	/*NewScanner函数解析os.File后可以通过input.scan()读取数据,读取的数据存储在input变量中的Text()中*/
	input := bufio.NewScanner(f)
	for input.Scan() {
		counts[input.Text()]++
	}
}
func main() {
	counts := make(map[string]int)
	files := os.Args[1:]
	if 1 == len(files) {
		countLines(os.Stdin, counts)
	} else {
		/*实现的是命令行输入的文件中所有重复的*/
		for _, arg := range files {
			f, err := os.Open(arg)
			if err != nil {
				fmt.Fprintf(os.Stderr, "dup2: %v\n", err)
				f.Close()
				continue
			}
			countLines(f, counts)
			f.Close()
		}
		for line, n := range counts {
			if n > 1 {
				fmt.Printf("%d\t%s\n", n, line)
			}
		}
	}
}
