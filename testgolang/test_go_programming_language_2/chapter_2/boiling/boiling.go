package main

import "fmt"

/*声明包级别常量*/
const boilingF = 212.0

func main() {
	var f = boilingF
	var c = (f - 32) * 5 / 9
	fmt.Printf("boiling point = %g ℃ \n", c)
}
