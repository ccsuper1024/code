package main

import (
	"fmt"
	"sort"
	"time"
)

type Employee struct {
	ID            int
	Name, Address string
	DoB           time.Time
	Position      string
	Salary        int
	ManagerID     int
}
type Point struct {
	x, y int
}

func AwardAnnualRaise(e *Employee) {
	e.Salary = e.Salary * 105 / 100
}

func main() {
	var a [3]int
	fmt.Println(a[0])
	fmt.Println(a[len(a)-1])

	//Print the indices and elements
	for i, v := range a {
		fmt.Printf("%d %d\n", i, v)
	}

	/*Print the element only*/
	for _, v := range a {
		fmt.Printf("%d\n", v)
	}

	type Currency int
	const (
		USD Currency = iota //美元
		EUR
		GRP
		RMB
	)
	symbol := [...]string{USD: "$", EUR: "€", GRP: "£", RMB: "¥"}
	fmt.Println(RMB, symbol[RMB])
	r := [...]int{99: -1}
	for i, va := range r {
		fmt.Printf("i = %u va = %u\n", i, va)
	}

	a1 := [2]int{1, 2}
	b1 := [...]int{1, 2}
	c1 := [2]int{1, 3}
	fmt.Println(a1 == b1, b1 == c1, a1 == c1)
	d := [3]int{1, 2, 3}
	fmt.Println(a == d)

	fmt.Println(".......................")
	ages := make(map[string]int)
	ages["chen"] = 1996
	v, ok := ages["bob"]
	if ok {
		fmt.Println(v)
	} else {
		fmt.Println(ages["chen"])
	}
	delete(ages, "bob")
	delete(ages, "chen")

	v, ok = ages["chen"]
	if ok {
		fmt.Println(v)
	} else {
		fmt.Printf("none\n")
	}
	fmt.Println(".......................")

	employee := map[string]int{
		"chenchao": 50000,
		"bob":      20000,
		"lvy":      25000,
	}
	for i, value := range employee {
		fmt.Printf("%s salary: %d\n", i, value)
	}

	employee["chenchao"] += 10000
	/*排序*/
	var names []string
	for name := range employee {
		names = append(names, name)
	}
	sort.Strings(names)
	for _, name := range names {
		fmt.Printf("%s\t%d\n", name, employee[name])
	}

	pp := &Point{1, 2}
	fmt.Printf("pp: x = %d y = %d\n", pp.x, pp.y)

	pp1 := new(Point)
	*pp1 = Point{3, 4}
	fmt.Printf("pp1: x = %d y = %d\n", pp1.x, pp1.y)

	q := Point{2131, 58456}
	p := Point{1, 2}
	n := Point{1, 2}
	fmt.Println(q == p, p == n, q == n)
	//fmt.Println(q == m)	  //必须是同一类型的比较

	// type address struct {
	// 	hostname string
	// 	port     int
	// }

	type Sphere struct {
		x, y, z int
	}
	type Circle struct {
		sphereInstace Sphere
		Point
		radius int
	}
	type Wheel struct {
		Circle
		Spokes int
	}

	var w Wheel
	w.radius = 2
	fmt.Printf("x = %d y = %d radius = %d\n", w.x, w.y, w.radius)
	w.sphereInstace.x = 45

}
