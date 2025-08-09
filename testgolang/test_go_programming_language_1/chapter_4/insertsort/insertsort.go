//用结构体实现的二叉树，实现插入排序
package main

import "fmt"

type tree struct {
	value       int
	left, right *tree
}

//构造树结构
func add(t *tree, value int) *tree {
	if t == nil {
		// Equivalent to return &tree{value: value}
		t = new(tree)
		t.value = value
		return t
	}
	if value < t.value {
		t.left = add(t.left, value)
	} else {
		t.right = add(t.right, value)
	}
	return t
}

//appendValues appends the elements of t to values in
//order and returns the resulting slice
//返回一个切片
func appendValues(values []int, t *tree) []int {
	if t != nil { //即当前节点不是叶子节点时
		//递归调用appendValuse()
		//这是个赋值语句，由于values变量是个int类型的切片，所以
		//函数返回的值通过赋值运算符会自动附加都切片的末尾
		values = appendValues(values, t.left)
		values = append(values, t.value)
		values = appendValues(values, t.right)
	}
	return values
}

//Sort sorts values in place
func Sort(values []int) {
	var root *tree
	for _, v := range values {
		//构造树结构
		root = add(root, v)
	}
	//values[:0]表示一个新的空切片，长度为0，容量与原始切片相同。这种用法
	//通常用于重新利用切片，将其长度重置为0，但保持底层数组的容量不变，
	//而且构造的新切片不会执行原始的切片底层数组，不会覆盖原始的切片底层数据。
	//appendValues(values[:0], root)将会把root中的元素追加到
	appendValues(values[:0], root)
}
func main() {
	//构造树
	var values []int
	values = append(values, 5, 3, 7, 1, 4, 6, 8)

	//调用sort函数对切片中的元素进行排序
	Sort(values)

	//打印排序后的结果
	fmt.Println(values)
}
