/*
使用go语言实现Linux中的wc工具
*/
package main

import (
	"bufio"
	"bytes"
	"fmt"
	"io"
	"os"
	"path/filepath"
)

type CountResult struct {
	Lines int
	Words int
	Bytes int
	Name  string
}

func counterReader(r io.Reader, name string) CountResult {
	var lines, words, bytesCount int
	//用bufio.scanner扫描行
	scanner := bufio.NewScanner(r)
	for scanner.Scan() {
		lines++
		line := scanner.Bytes()
		bytesCount += len(line) + 1 //加上换行符

		//用另一个scanner统计单词
		wordScanner := bufio.NewScanner(bytes.NewReader(line))
		wordScanner.Split(bufio.ScanWords)

		for wordScanner.Scan() {
			words++
		}
	}
	return CountResult{
		Lines: lines,
		Words: words,
		Bytes: bytesCount,
		Name:  name,
	}

}

// 格式化输出统计结果
func printResult(r CountResult) {
	fmt.Printf("%8d %8d %8d %s\n", r.Lines, r.Words, r.Bytes, r.Name)
}
func main() {
	args := os.Args[1:]

	//如果没有参数，从标准输入中读取数据
	if len(args) == 0 {
		res := counterReader(os.Stdin, "stdin")
		printResult(res)

		return
	}

	//否则，对每个文件进行统计
	for _, path := range args {
		file, err := os.Open(path)
		if err != nil {
			fmt.Fprintf(os.Stderr, "打开文件失败:%v\n", err)
			continue
		}
		defer file.Close()

		res := counterReader(file, filepath.Base(path))
		printResult(res)
	}
}
