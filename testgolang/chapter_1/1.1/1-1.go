// 修改echo程序，使其能够打印os.Args[0],即被执行程序本身的名字
package main

import (
	"fmt"
	"os"
	"strings"
)

func main() {
	fmt.Println(strings.Join(os.Args[:], " "))
}
