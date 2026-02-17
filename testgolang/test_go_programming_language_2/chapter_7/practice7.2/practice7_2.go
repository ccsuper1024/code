/*
写一个带有如下函数签名的函数CountingWriter，传入一个io.Writer接口类型，返回一
个新的Writer类型把原来的Writer封装在里面和一个表示写入新的Writer字节数的int64类型指针
func CountingWriter(w io.Writer) (io.Writer, *int64)
*/
package main

import (
	"fmt"
	"io"
	"os"
)

type countingWriter struct {
	writer io.Writer
	count  *int64
}

// countingWrite类型实现io.Write接口,类似于继承了io.Writer对象
func (cw *countingWriter) Write(p []byte) (int, error) {
	n, err := cw.writer.Write(p)
	if err != nil {
		fmt.Println("cw.writer.Write方法 Failed !!!")
		os.Exit(1)
	}
	*cw.count += int64(n)
	return n, err

}

// CountingWriter函数返回了一个封装了计数功能的Writer和计数指针
func CountingWriter(r io.Writer) (io.Writer, *int64) {
	var n int64
	cw := &countingWriter{writer: r, count: &n}
	return cw, &n
}

func main() {
	w, count := CountingWriter(io.Discard) //io.Discard 是个丢弃输出的Writer

	w.Write([]byte("hello"))
	w.Write([]byte(" World"))

	fmt.Println("Total bytes written:", *count)

}
