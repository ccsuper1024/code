package tempconv

import (
	"flag"
	"fmt"
)

type Celsius float64
type Fahrenheit float64

func CToF(c Celsius) Fahrenheit {
	return Fahrenheit(c)
}

func FToC(f Fahrenheit) Celsius {
	return Celsius(((f - 32.0) * 5.0 / 9.0))
}

func (c Celsius) String() string {
	return fmt.Sprintf("%g°C", c)
}

/*内嵌另一个组合类型*/
type celsiusFlag struct {
	Celsius
}

func (f *celsiusFlag) Set(s string) error {
	var unit string
	var value float64

	fmt.Sscanf(s, "%f%s", &value, &unit)
	switch unit {
	case "C", "°C":
		f.Celsius = Celsius(value)
		return nil
		// go语言自动break，不需要break关键字
	case "F", "°F":
		f.Celsius = FToC(Fahrenheit(value))
		return nil
	}
	return fmt.Errorf("invalid temperature %q", s)
}

func CelsiusFlag(name string, value Celsius, usage string) *Celsius {
	f := celsiusFlag{value} //内部内嵌了一个Celsius类型
	flag.CommandLine.Var(&f, name, usage)
	return &f.Celsius
}
