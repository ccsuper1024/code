package main

import "fmt"

type Celsius float64    //摄氏温度
type Fahrenheit float64 //华氏温度

const (
	AbsoluteZeroC Celsius = -237.15 //绝对零度
	FreezingC     Celsius = 0       //结冰点温度
	BoilingC      Celsius = 100     //沸水温度
)

func CToF(c Celsius) Fahrenheit {
	return Fahrenheit(c*9/5 + 32)
}

func FToC(f Fahrenheit) Celsius {
	return Celsius((f - 32) * 5 / 9)
}

func main() {
	fmt.Printf("%g\n", BoilingC-FreezingC)
	BoilingF := CToF(BoilingC)
	fmt.Printf("%g\n", BoilingF-CToF(FreezingC))
	// fmt.Printf("%g\n", BoilingF-FreezingC)
}
