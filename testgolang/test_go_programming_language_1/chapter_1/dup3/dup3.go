//前两个dup程序都是以"流"模式来读取输入的，还有一种方法是一口气把全部的输入数据读取
//到内存中，一次分割为多行，然后处理他们。
//所以可以用io/ioutil包中的ReadFile函数来一次读取文件的全部内容。
//strings.Split函数来把字符串分割成子串
package main

import (
	"fmt"
	"io/ioutil"
	"os"
	"strings"
)

func main() {
	counts := make(map[string]int)
	//range总是返回两个值，但第一个值我们不需要，所以用空白标识符来存储
	//空白标识符表示丢弃这些数据
	for _, filename := range os.Args[1:] {
		data, err := ioutil.ReadFile(filename)
		if err != nil {
			fmt.Fprintf(os.Stderr, "dup3: %v\n", err)
			continue //直接跳到for循环的下一此迭代
		}
		//按行切分文件中的数据
		//基于range的循环
		for _, line := range strings.Split(string(data), "\n") {
			counts[line]++
		}
	}
	for line, n := range counts {
		if n >= 1 {
			fmt.Printf("%d\t%s\n", n, line)
		}
	}
}
