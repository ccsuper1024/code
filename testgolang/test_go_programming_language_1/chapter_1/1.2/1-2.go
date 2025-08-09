//修改echo程序，使其打印每个参数的索引和值，每个一行
package main

import (
	"fmt"
	"os"
	"strings"
)

func main() {
	fmt.Println(strings.Join(os.Args[:], "\n"))
}
