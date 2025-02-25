/*  @file @brief @author chenchao @brief */
package main

import (
	"flag"
	"fmt"
	"strings"
)

//变量类型自动推断

var n = flag.Bool("n", false, "omit trailing newline") //n的类型为*bool，即指向bool类型的指针
var sep = flag.String("s", " ", "separator")           //sep的类型为*string，即指向string类型的指针

func main() {
	//Parse()解析来自os的命令行。参数[1:]，即不包括argv[0]--程序名。
	//必须在定义所有标志之后，和程序访问之前调用
	flag.Parse()
	//Args()返回非标志的命令行参数
	fmt.Println("sep = ", *sep)
	fmt.Println(strings.Join(flag.Args(), *sep)) //
	if !*n {                                     //!*n 中先对n解引用。获取n的值所指向地址的值，再取反
		fmt.Println()
	}
    fmt.Println(new(int))
}
