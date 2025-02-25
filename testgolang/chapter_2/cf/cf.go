//CF converts its numeric argument to Celsius and Fahrenheit
package main

import (
	"fmt"
	"hello/chapter_2/tempconv0"
	"os"
	"strconv"
)

func main() {
	for _, arg := range os.Args[1:] { //获取除程序名外所有参数
		t, err := strconv.ParseFloat(arg, 64) //将字符串转成给定位数的浮点型
		if err != nil {
			fmt.Fprintf(os.Stderr, "cf: %v\n", err)
			os.Exit(1)
		}
		f := tempconv0.Fahreheit(t)
		c := tempconv0.Celsius(t)
		fmt.Printf("%s = %s, %s = %s\n", f, tempconv0.FToC(f), c,
			tempconv0.CtoF(c))
	}
}
