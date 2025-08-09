//package tempconv performs Celsius and Fahreheit temperature computations
package tempconv0

//定义类型别名 格式为：type 别名名称 类型名
type Celsius float64   //摄氏温度
type Fahreheit float64 //华氏温度

const (
	//变量声明格式：变量名 类型名	或者var 变量名 类型名
	AbsoluteZeroC Celsius = -273.15 //绝对零度
	FreezingC     Celsius = 0       //结冰点温度
	BoilingC      Celsius = 100     //沸水温度
)


//return 关键词后的是类型转换方法不是函数调用
func CtoF(c Celsius) Fahreheit { return Fahreheit(c*9/5 + 32) }
func FToC(f Fahreheit) Celsius { return Celsius(f - 32*5/9) }
