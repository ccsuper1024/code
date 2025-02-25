//Boiling prints the boiling point of water
package main

import "fmt"

//包变量，在包一级范围声明语句声明的。可以被外部访问
const boilingF = 212.0

//注意，go语言的源码编译规则不是像c/c++一样的一次定义原则。所以，
//go的函数是定义即声明的，且它的顺序和编译后的二进制代码顺序无关
//所以，可以看到，即使在main函数之后只定义一次的函数也可以被其之前的
//main函数调用
func main() { //函数声明语句
	var f = boilingF //局部变量，块变量
	var c = (f - 32) * 5 / 9
	fmt.Printf("boiling point = %g F or %g C\n", f, c)
}
