package main

import (
	"fmt"
	"io"
	"os"
	"reflect"
)

func main() {
	t := reflect.TypeOf(3)
	fmt.Println(t.String())
	fmt.Println(t)

	var w io.Writer = os.Stdout
	fmt.Println(reflect.TypeOf(w))

	fmt.Printf("%T\n", 3)

	v := reflect.ValueOf(3)
	fmt.Println(v)
	fmt.Printf("%v\n", v)
	fmt.Println(v.String())

	fmt.Println(v.Type())

	v = reflect.ValueOf(3)
	x := v.Interface()
	i := x.(int)
	fmt.Printf("%d\n", i)
}
