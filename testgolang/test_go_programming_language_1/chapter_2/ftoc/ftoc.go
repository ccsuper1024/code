//Ftoc prints two Fahrenheit to Celsius conversions
//包的声明语句
package main

//导入依赖包
import "fmt"

//函数的声明语句，go中的函数和变量都是一等公民
func main() {
	//常量声明 且是块变量
	const freezingF, boilingF = 32.0, 212.0
	fmt.Printf("%g F = %g C\n", freezingF, fToC(freezingF))
	fmt.Printf("%g F = %g C\n", boilingF, fToC(boilingF))
}

func fToC(f float64) float64 {
	return (f - 32) * 5 / 9
}
