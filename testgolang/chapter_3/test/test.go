//test go language
package main

import (
	"fmt"
	"math"
)

var f float32 = 16777216 //1 << 24	左移24位

func main() {
	fmt.Println(f == f+1)
	const e = 2.71818 //(approximately)
	fmt.Println(e)

	for x := 0; x < 8; x++ {
		fmt.Printf("x = %d e^x = %8.3f\n", x, math.Exp(float64(x)))
	}

	var z float64
	fmt.Println(z, -z, 1/z, z/z) //0 -0 +Inf NaN
	nan := math.NaN()
	//NaN非数和任何数都不相等。在浮点数中NaN,正无穷大和负无穷大都不是唯一的，每个
	//都有非常多种的bit模式表示
	fmt.Println(nan == nan, nan < nan, nan > nan) //false ,false ,false
}

//在go语言中无论是在何处定义函数，都可以被包中其他函数调用，即不必遵守c/c++
//中的先声明，后定义，再调用的顺序
//func compute() (value float64, ok bool) {
//}
