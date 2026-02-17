package main

import (
	"fmt"
	"myGo2/chapter_7/bytecounter/bytecounter"
)

func main() {
	var c bytecounter.ByteCounter
	c.Write([]byte("hello"))
	fmt.Println(c)

	c = 0
	var name = "Dolly"
	fmt.Fprintf(&c, "hello,%s", name)
	fmt.Println(c)

	fmt.Printf("It is right\n")
}
