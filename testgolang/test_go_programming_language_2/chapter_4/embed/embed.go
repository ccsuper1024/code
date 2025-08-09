package main

import "fmt"

// 导出变量
type Point struct {
	x, y int
}
type Circle struct {
	Point  //匿名成员
	Radius int
}
type Wheel struct {
	Circle //匿名成员
	Spokes int
}

func main() {
	var w Wheel
	w = Wheel{Circle{Point{8, 8}, 5}, 20}

	w.Circle.y = 50
	w = Wheel{
		Circle: Circle{
			Point:  Point{x: 8, y: 8},
			Radius: 5,
		},
		Spokes: 20, //	Note:trailing comma necessary here (and at Radius)
	}
	fmt.Printf("%#v\n", w)

	w.x = 42 //缩短访问路径
	fmt.Printf("%#v\n", w)
}
