package popcount

var pc [256]byte

func init() {
	for i := range pc {
		pc[i] = pc[i/2] + pc[i&1]
	}
}

// func PopCount(x uint64) int {
// 	return int(pc[byte(x>>(0*8))] +
// 		pc[byte(x>>(1*8))] +
// 		pc[byte(x>>(2*8))] +
// 		pc[byte(x>>(3*8))] +
// 		pc[byte(x>>(4*8))] +
// 		pc[byte(x>>(5*8))] +
// 		pc[byte(x>>(6*8))] +
// 		pc[byte(x>>(7*8))])
// }

// 用于统计一个 byte 中有多少个 1
func PopCount(b byte) int {
	count := 0
	for b != 0 {
		b &= b - 1 // 清除最低位的1
		count++
	}
	return count
}
