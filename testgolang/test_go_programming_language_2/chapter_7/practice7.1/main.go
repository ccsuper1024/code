package main

import (
	"fmt"
	"io"
	"myGo2/chapter_7/practice7.1/counter"
)

func main() {
	text := "Go is expressive, concise,clean and effecient.\n Go compiles quickly to machine code.\n"

	var bc counter.ByteCounter
	var wc counter.WordCounter
	var lc counter.LineCounter

	bc.Write([]byte(text))
	wc.Write([]byte(text))
	lc.Write([]byte(text))

	fmt.Printf("字节数：%d\n", bc)
	fmt.Printf("单词数：%d\n", wc)
	fmt.Printf("行数：%d\n", lc)

	//或直接使用辅助函数
	fmt.Printf("\n直接函数调用\n")
	fmt.Printf("单词数：%d\n", counter.CountWords(text))
	fmt.Printf("行数：%d\n", counter.CountLines(text))

	//复用Write 接口
	fmt.Printf("\n使用Write接口写入更多内容\n")
	io.WriteString(&wc, "Another line wite words")
	io.WriteString(&lc, "One\nTwo\nThree\n")
	fmt.Printf("累计单词数：%d\n", wc)
	fmt.Printf("累计行数：%d\n", lc)
}
