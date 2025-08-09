package main

import (
	"fmt"
	"math"
	"strconv"
)

func main() {
	var x uint8 = 1<<1 | 1<<5
	var y uint8 = 1<<1 | 1<<2

	fmt.Printf("%08b\n", x)
	fmt.Printf("%08b\n", y)
	fmt.Printf("%08b\n", ^1)

	fmt.Printf("%08b\n", x&y)
	fmt.Printf("%08b\n", x|y)

	medals := []string{"gold", "silver", "bronze"}
	for i := len(medals) - 1; i >= 0; i-- {
		fmt.Printf("i = %d\n", i)
		fmt.Println(medals[i])
	}

	ascii := 'a'
	unicode := '国'
	unicode2 := '家'
	newline := '\n'

	fmt.Printf("%d %[1]c %[1]q\n", ascii)
	fmt.Printf("%d %[1]c %[1]q\n", unicode)
	fmt.Printf("%d %[1]c %[1]q\n", unicode2)
	fmt.Printf("%d  %[1]q\n", newline)

	const e = 2.71828
	const Avogadro = 6.0221445e23 //阿伏伽德罗常数
	const Planck = 6.62606957e-34 //普朗克常数

	fmt.Printf("%f %[1]e %[1]g\n", e)
	fmt.Printf("%f %[1]e %[1]g\n", Avogadro)
	fmt.Printf("%f %[1]e %[1]g\n", Planck)
	for x := 0; x < 8; x++ {
		fmt.Printf("x = %d e^x = %8.3f %g[]\n", x, math.Exp(float64(x)), math.Exp(float64(x)))
	}

	var z float64
	fmt.Println(z, -z, 1/z, -1/z, z/z)

	nan := math.NaN()
	fmt.Println(nan == nan, nan < nan, nan > nan)

	var cx complex128 = complex(1, 2)
	var cy complex128 = complex(3, 4)
	fmt.Println(x * y)
	fmt.Println(real(cx * cy))
	fmt.Println(imag(cx * cy))

	x1 := 123
	y1 := fmt.Sprintf("%d", x1)
	fmt.Println(y1, strconv.Itoa(x1))

	fmt.Println(strconv.FormatInt(int64(x1), 2))
	s1 := fmt.Sprintf("x=%b", x)
	fmt.Printf("s1 = %s\n", s1)

	x2, err := strconv.Atoi("123")
	if err != nil {
		fmt.Printf("strconv.Atoi Failed!!")
	}
	fmt.Printf("x2 = %d\n", x2)

	x3, err := strconv.ParseInt("123", 10, 64) //base 10, up to 64 bits
	if err != nil {
		fmt.Printf("strconv.ParseInt Failed!!")
	}
	fmt.Printf("x3 = %d\n", x3)

	type WeekDay int
	const (
		Sunday WeekDay = iota
		Monday
		Tuesday
		Wednesday
		Thursday
		MAX
	)
	fmt.Printf("Sunday = %d  Monday = %d\n", Sunday, Monday)
	var f float64 = 212
	fmt.Println(5.0 / 9.0 * (f - 32))
}
