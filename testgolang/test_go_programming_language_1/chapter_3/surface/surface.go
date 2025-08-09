//通过浮点计算生成图形。它是带有两个参数的z=f(x,y)函数的三维形式，
//使用了缩放矢量图形(SVG)格式输出，SVG是一个用于矢量线绘制的XML标准
package main

import (
	"fmt"
	"math"
)

const (
	width, height = 600, 320            //canvas size in pixels
	cells         = 100                 //number of grid cells
	xyrange       = 30.0                //axis ranges(-xyrang +xyrange)
	zscale        = height * 0.4        //pixels per z unit
	angle         = math.Pi / 6         //angle of x, y axes(=30°)
	xyscale       = width / 2 / xyrange //pixels per x or y unit
)

var sin30, cos30 = math.Sin(angle), math.Cos(angle) //sin(30°), cos(30°)

func main() {
	fmt.Printf("<svg xmlns='http://www.w3.org/2000/svg' "+
		"style='stroke: grey; fill: white; stroke-width: 0.7' "+
		"width='%d' height='%d'>", width, height)
	for i := 0; i < cells; i++ {
		for j := 0; j < cells; j++ {
			ax, ay := corner(i+1, j)
			bx, by := corner(i, j)
			cx, cy := corner(i, j+1)
			dx, dy := corner(i+1, j+1)
			fmt.Printf("<polygon points='%g,%g,%g,%g,%g,%g,%g,%g'/>\n",
				ax, ay, bx, by, cx, cy, dx, dy)
		}
	}
	fmt.Println("</svg>")
}
func corner(i int, j int) (float64, float64) {
	//Find point (x, y) at corner of cell (i, j)
	x := xyrange * (float64(i)/cells - 0.5)
	y := xyrange * (float64(j)/cells - 0.5)

	//Compute surface height z
	z := f(x, y)

	//Project (x,y,z) is ometrically onto 2-D SVG canvas (sx, sy)
	sx := width/2 + (x-y)*cos30*xyscale
	sy := height/2 + (x+y)*sin30*xyscale - z*zscale
	return sx, sy
}

//参数列表中，如果多个参数的类型相同，可以只在最后一个参数后声明类型
func f(x, y float64) float64 {
	r := math.Hypot(x, y) //distance from (0,0)
	return math.Sin(r) / r
}
