/*
使用来自ByteCounter的思路，实现一个针对单词和行数的计数器， 你会发现bufio.ScanWords非常有用。
*/
package counter

import (
	"bufio"
	"bytes"
)

type ByteCounter int

func (c *ByteCounter) Write(p []byte) (int, error) {
	*c += ByteCounter(len(p)) //Convert int to ByteCounter
	return len(p), nil
}

// WordCounter统计写入的单词数
type WordCounter int

// 方法的接收者是指针，表示可以在方法的内部修改调用者的值
// 以下的代码逻辑是非常典型的面对对象的处理。将输入的字节流用对象包裹，后续的处理都在对象内部的函数中
func (c *WordCounter) Write(p []byte) (int, error) {
	//bytes.NewReader将字节切片包装成一个io.Reader对象
	//bufio.NewScanner接收一个io.Reader，返回一个*bufio.Scanner 用于输入分成一个个“Token”逐个读取。
	//第一行代码就是把一个字节切片包装成一个Scanner对象，便于后续处理
	scanner := bufio.NewScanner(bytes.NewReader(p))
	//这一行只是把Scanner对象的分割函数设置成bufio.ScanWords
	//bufio.ScanWords的规则是:跳过空白，范湖连续的费恐怖字节为一个token
	scanner.Split(bufio.ScanWords)

	count := 0
	//scanner.Scan函数会对scanner包装的字节切片调用Split函数等操作
	//直到内部的数据读完了，或者扫描过程出错时结束
	/*
		scanner.Scan()内部逻辑
		bufio.Scanner是一个循环读取的状态机
		1、从底层的io.Reader对象中获取一部分字节
		2、调用scanner对象中设置的Split函数，它会在缓冲区中虚招下一个token的起止位置，并返回token的长度、token内容、以及是否有错误
		3、如果找到token，则存储该token，返回true，for循环继续
		4、如果读完或遇到错误，则返回false，for循环结束
	*/
	for scanner.Scan() {
		count++
	}
	//Go 允许在具有相同底层类型的两个类型之间进行显式转换
	//但不允许隐式转换，如 *c += count
	//这是go的设计哲学，显式由于隐式
	*c += WordCounter(count)
	return len(p), scanner.Err()
}

// LineCounter 统计写入的行数
type LineCounter int

func (c *LineCounter) Write(p []byte) (int, error) {
	scanner := bufio.NewScanner(bytes.NewReader(p))
	scanner.Split(bufio.ScanLines)
	count := 0
	for scanner.Scan() {
		count++
	}
	*c += LineCounter(count)
	return len(p), scanner.Err()
}

//辅助函数

// CountWords 也可以提供一个辅助函数，方便直接统计字符串中的单词数
func CountWords(s string) int {
	var wc WordCounter
	wc.Write([]byte(s))
	return int(wc)
}

// CountLines 统计字符串中的行数
func CountLines(s string) int {
	var lc LineCounter
	lc.Write([]byte(s))
	return int(lc)
}
