//dup 的第一个版本打印标准输入中多次出现的行
//Dup1 prints the text of each line that appears more than
//once in the standard of input, precedee by its count
package main

import (
	"bufio"
	"fmt"
	"os"
)

func main() {
	//make内置函数创建一个kongmap
	counts := make(map[string]int)
	//input的类型为*bufio.Scanner
	//从标准输入读取数据
	input := bufio.NewScanner(os.Stdin)
	for input.Scan() {
		//有新的键值就增加一个键，旧的键则增加键对应的值
		counts[input.Text()]++
	}
	//Note：ignoring potential errors from input.Err()
	for line, n := range counts {
		if n > 1 {
			fmt.Printf("%d\t%s\n", n, line)
		}
	}
}
