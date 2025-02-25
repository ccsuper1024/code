//Echo1 prints its command-line arguments
package main

import (
	"fmt"
	"os"
)

//参数列表为(args []string).
//参数列表中如果参数的类型相同，可以在最后一个参数后面
//声明类型即可。不建议如此，和其他程序语言不合，增加记忆负担
//函数返回值类型为 string
func echo(args []string) string {
	//Go语言没有未初始化的变量，所有变量都会被自动初始化
	//如果变量未被显式初始化，则会被隐式初始化为各类型的初始值
	//string类型的初始值为空字符串
	var s, sep string
	//go语言只有后置自增运算符
	for i := 1; i < len(args); i++ {
		s += sep + args[i]
		sep = " "
	}
	return s
}
func main() {
	//os.Args是个变量，类型是string的切片。即一个简版的动态数组
	//其中存储着包含程序名的命令行参数
	fmt.Println(echo(os.Args))
}
