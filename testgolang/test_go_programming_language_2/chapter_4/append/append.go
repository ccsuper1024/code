package main

import "fmt"

//	func appendInt(x []int, y int) []int {
//		var tmp []int
//		zlen := len(x) + 1
//		// fmt.Println(len(x))
//		// fmt.Println(cap(x))
//		if zlen <= cap(x) {
//			/*there is enough space to grow, Exend the slice*/
//			tmp = x[:zlen]
//		} else {
//			/*
//				There is insufficient space ,Allocate a new array
//				Grow by doubling , for amortized linear complexity
//			*/
//			zcap := cap(x)
//			if zcap == 0 {
//				zcap = 1
//			}
//			for zcap < zlen {
//				zcap *= 2
//			}
//			tmp = make([]int, zlen, zcap)
//			copy(tmp, x)
//		}
//		tmp[len(x)] = y
//		return tmp
//	}
func appendInt(x []int, y int) []int {
	var z []int
	zlen := len(x) + 1
	if zlen <= cap(x) {
		// There is room to grow. Extend the slice.
		z = x[:zlen]
	} else {
		// There is insufficient space. Allocate a new array.
		// Grow by doubling, for amortized linear complexity.
		zcap := zlen
		if zcap < 2*len(x) {
			zcap = 2 * len(x)
		}
		z = make([]int, zlen, zcap)
		copy(z, x) // a built‐in function; see text
	}
	z[len(x)] = y
	return z
}

func main() {
	var x, y []int
	for i := range 10 {
		y = appendInt(x, i)
		fmt.Printf("%d cap = %d\t%v\n", i, cap(y), y)
		x = y
	}

	x = append(x, 1)
	x = append(x, 2, 3)
	x = append(x, x...)
	fmt.Printf("x = %v\n", x)
}
