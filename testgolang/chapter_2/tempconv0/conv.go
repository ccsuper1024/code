package tempconv0

import "fmt"

//定义一个方法。注意：go语言中方法不等于函数
//方法的格式如下：func关键字 + 参数列表 + 方法名() + 返回值类型 + 方法体
func (c Celsius) String() string   { return fmt.Sprintf("%g℃", c) }
func (f Fahreheit) String() string { return fmt.Sprintf("%g℉", f) }

//因为vscode有go语言插件。它会自动添加使用过的包.如import "fmt",并且保存后会
//自动修改格式
