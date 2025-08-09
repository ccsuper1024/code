package main

import (
	"bufio"
	"fmt"
	"os"
)

func main() {
	counts := make(map[string]int)
	input := bufio.NewScanner(os.Stdin)

	for input.Scan() { //每调用一次scan()函数都会从input变量关联的输入中读取一行输入，读取到的值放入input.Text()中
		counts[input.Text()]++ //golang只有后缀自增
	}

	//Note: ignoring potenial errors from input.Err()
	for line, n := range counts {
		if n > 1 {
			fmt.Printf("%d\t%s\n", n, line)
		}
	}
}
