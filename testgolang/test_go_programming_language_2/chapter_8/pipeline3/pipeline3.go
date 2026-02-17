package main

import "fmt"

var count = 100

/*参数为单向的通道，只能发送.发送的数据是一个整数*/
func counter(out chan<- int) {
	for i := 0; i < count; i++ {
		out <- i
	}
	close(out) //注意：单向发送通道是可以关闭的，但它和双向通道的关闭是不一样的
}

func squarer(out chan<- int, in <-chan int) {
	for v := range in {
		v *= v
		out <- v
	}
	close(out) //注意：此处只关闭了单向发送通道，没有关闭单向接受通道
}

func printer(in <-chan int) {
	for v := range in {
		fmt.Println(v)
	}
	//注意：此处没有挂壁单向接受通道，此通道在没有发送者后会被GC回收
}

func main() {
	/*创建双向通道*/
	naturals := make(chan int)
	squares := make(chan int)

	go counter(naturals)
	go squarer(squares, naturals)
	printer(squares)
}
