/*
修改dup2.go,要求在出现重复行时打印文件名称
*/
package main

import (
	"bufio"
	"fmt"
	"os"
)

/*go语言默认使用地址传递参数,所以在函数内部修改参数在外部是可见的*/
func countLinesWithFileName(f *os.File, counts map[string]int, file map[string]map[string]bool) {
	/*NewScanner函数解析os.File后可以通过input.scan()读取数据,读取的数据存储在input变量中的Text()中*/
	input := bufio.NewScanner(f)
	for input.Scan() {
		line := input.Text()
		counts[line]++

		/*当行重复次数大于1时,将文件记录在map中*/
		if nil == file[line] {
			file[line] = make(map[string]bool)
		}
		file[line][f.Name()] = true
	}
}
func countLines(f *os.File, counts map[string]int) {
	/*NewScanner函数解析os.File后可以通过input.scan()读取数据,读取的数据存储在input变量中的Text()中*/
	input := bufio.NewScanner(f)
	for input.Scan() {
		counts[input.Text()]++
	}
}
func main() {
	file := make(map[string]map[string]bool)
	counts := make(map[string]int)
	files := os.Args[1:]

	if 0 == len(files) {
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
			countLinesWithFileName(f, counts, file)
			f.Close()
		}
	}

	fmt.Printf("|%-20s|%-s\t|%-s|\n", "文本", "重复次数", "文件名")
	for line, n := range counts {
		if n > 1 {
			width := 0
			if len(line) < 20 {
				width = 20
			} else {
				width = len(line)
			}
			fmt.Printf("%-*s\t%d\t", width, line, n)
			for filename := range file[line] {
				fmt.Printf("%s ", filename)
			}
			fmt.Println()
		}
	}
}
