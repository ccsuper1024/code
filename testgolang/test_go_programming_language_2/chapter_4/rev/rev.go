package main

import (
	"bytes"
	"fmt"
)

/*reverse function reverses a slice of ints in place.*/
/*slice作为实参传递时，因为slice值包含指向第一个slice元素的指针，所以函数内部修改slice也就修改了slice底层的数组元素:w
 */
func reverse(s []int) {
	for i, j := 0, len(s)-1; i < j; i, j = i+1, j-1 {
		s[i], s[j] = s[j], s[i]
	}
}
func equal(x, y []string) bool {
	if len(x) != len(y) {
		return false
	}
	for i := range x {
		if x[i] != y[i] {
			return false
		}
	}
	return true
}

func main() {
	/*这是一个数组*/
	a := [...]int{0, 1, 2, 3, 4}
	fmt.Printf("a = %v\n", a)
	reverse(a[:])
	fmt.Printf("a = %v\n", a)

	/*
		讲一个slice序列左旋转n个元素的方法是三次调用reverse反转函数，第一次反转开头的n个元素，然后是反转剩下的元素，最后反转整个
		slice的元素。（如果是向右循环旋转，则将第三个函数调用移到第一个调用位置接可以了）
	*/
	/*这是一个slice动态序列*/
	s := []int{0, 1, 2, 3, 4, 5}
	/*Rotate s left by two positons*/
	fmt.Printf("s = %v\n", s)
	reverse(s[:2])
	reverse(s[2:])
	reverse(s)
	fmt.Printf("s = %v\n", s)
	/*
		数组和slice动态序列的字面量初始化语法很相似，但是两者的区别在于数组的大小是固定的，虽然上面没有用明确指定数组的大小，而是
		由编译器隐式推导出来的。而下面的s slice动态序列根本就没有指定大小，它的大小是动态的
	*/

	b := []byte{0x12, 0x34, 0x16, 0x78}
	d := []byte{0x12, 0x34, 0x25, 0x95}
	fmt.Printf("b == d ? %t\n", bytes.Equal(d, b))

	summer := []int{}
	if summer == nil {
		fmt.Printf("summer slice is not initialize\n")
	}
	summer = []int(nil) //注意：这不是一个字面量初始化语法，而是一个类型转换表达式，它把nil值转换为一个[]int切片，再赋值给summer slice切片
	if summer == nil {
		fmt.Printf("after initialize by []int(nil) summer slice is nil now\n")
	}

	fmt.Printf("")

	f := make([]string, 4, 6)
	f[0] = "123"
	f = []string{"123", "456"}

	fmt.Printf("f = %v\n", f[:])
	fmt.Printf("g = %v\n", make([]int, 4)[4:])
}
