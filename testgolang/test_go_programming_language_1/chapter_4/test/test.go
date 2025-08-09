//test
package main

import "fmt"

type Currency int

const (
	USD Currency = iota //iota常量生成器
	EUR
	GBP
	RMB
)

type Point struct {
	x, y int //未导出变量
}

type Circle struct {
	Point      //匿名成员，它是导出还是不导出的要看它的成员名是不是首字母大写的
	Radius int //导出变量
}

type Wheel struct {
	Circle
	Spokes int
}

func main() {
	//symbol := [...]string{USD: "$", EUR: "€", GBP: "￡", RMB: "￥"}
	////fmt.Printf format %c has arg symbol[USD] of wrong type
	//fmt.Printf("the first char is : %s\n", symbol[USD])

	//number := [...]int{0: 1, 1: 5, 2: 4}
	//fmt.Printf("the first number is %d\n", number[2])

	//var runes []rune //rune切片类型,元素都是空的
	//for _, r := range "hello,世界" {
	//	runes = append(runes, r)
	//}
	//fmt.Printf("%q\n", runes) //"['H' 'e' 'l' 'l' 'o' ',' ' ' '世' '界']"['H']

	w := Wheel{Circle{Point{8, 8}, 5}, 20}
	w2 := Wheel{Circle: Circle{
		Point:  Point{x: 8, y: 8},
		Radius: 5,
	},
		Spokes: 20, //Note: trailing comma necessary here(and at Radius)
	}
	fmt.Printf("%#v\n", w)
	//Output:
	// Wheel{Circle:Circle{Point:Point{X:8, Y:8}, Radius:5}, Spokes:20}
	w.x = 42 //对匿名成员的成员的直接访问

	fmt.Printf("%#v\n", w2)
	// Output:
	// Wheel{Circle:Circle{Point:Point{X:42, Y:8}, Radius:5}, Spokes:20}
}
