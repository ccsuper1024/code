/*
为了不重复操作，使用go程序创建指定的目录名和此目录下的指定文件名(以.go结尾)
需求：程序应该能接受1个，2个或者3个命令行参数。
当只有1个命令行参数时，只创建此参数的目录及目录下的文件名。
如果输入2个参数时，第一个参数指明创建的目录名，第二个参数指明目录下的文件名。
如果输入3个参数时，第一个参数指明创建的目录名，第二个参数指明目录下的文件名，第三个参数表示从第一个目录开始到最后结束的序号
如createSpecifiedDirectoriesAndFile practice7.1 practice7_1 17,则创建 practice7.1/practice7_1.go practice7.2/practice7_2.go ... practice7.16/practice7_16.go
*/
package main

import (
	"fmt"
	"log"
	"os"
	"strconv"
)

func main() {
	args := os.Args[1:]
	//输出变量的类型
	fmt.Printf("type of args: %T", args)
	//args是一个字符串切片，每个元素都是一个字符串，其值是命令行参数
	var err error
	if len(args) == 1 {
		//创建目录
		err = os.Mkdir(args[1], 0777)
		if err != nil {
			fmt.Println("os.Mkdir Failed!!")
			os.Exit(1)
		}
		//创建目录下的文件名
		var tmpFileName = args[1] + "/" + args[1] + ".go"
		_, err = os.Create(tmpFileName)
		if err != nil {
			fmt.Println("os.Create Failed!!!")
			os.Exit(1)
		}
		fmt.Println("Jos is successful")
	} else if len(args) == 2 {
		//创建目录
		err = os.Mkdir(args[1], 0777)
		if err != nil {
			fmt.Println("os.Mkdir Failed!!")
			os.Exit(1)
		}
		//创建目录下的文件名
		var tmpFileName = args[1] + "/" + args[2] + ".go"
		_, err = os.Create(tmpFileName)
		if err != nil {
			fmt.Println("os.Create Failed!!!")
			os.Exit(1)
		}
		fmt.Println("Jos is successful")
	} else if len(args) == 3 {
		n, err := strconv.Atoi(args[3])
		if err != nil {
			fmt.Println("strconv.Atio failed !!!")
		}
		for i := 0; i < n; i++ {
			//创建目录
			err = os.Mkdir(args[1], 0777)
			if err != nil {
				fmt.Println("os.Mkdir Failed!!")
				os.Exit(1)
			}
			//创建目录下的文件名
			var tmpFileName = args[1] + "/" + args[2] + strconv.Itoa(i) + ".go"
			_, err = os.Create(tmpFileName)
			if err != nil {
				fmt.Println("os.Create Failed!!!")
				os.Exit(1)
			}
		}

		fmt.Println("Jos is successful")

	} else {
		log.Fatal("错误的命令行参数个数，请输入正确的命令行参数。此命令只接受1或者2或者3个命令行参数")
	}
}
