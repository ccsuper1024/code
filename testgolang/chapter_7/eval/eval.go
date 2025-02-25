//表达式求值
//Package eval provides an expression evalator

package eval

import (
	"fmt"
	"math"
	"strings"
	"testing"
)

//A Var identifies a variable, e.g ,x
type Var string

// Check implements Expr.
func (v Var) Check(vars map[Var]bool) error {
	vars[v] = true
	return nil
}

//A literal is a numeric constant, e.g., 3.141
type literal float64

// Check implements Expr.
func (literal) Check(vars map[Var]bool) error {
	return nil
}

//A unary represents a unary operator expression, e.g , -x
type unary struct {
	op rune //one of '+', '-'
	x  Expr
}

// Check implements Expr.
func (u unary) Check(vars map[Var]bool) error {
	/*
		func strings.ContainsRune(s string, r rune) bool
		ContainsRune reports whether the Unicode code point r is within s
	*/
	if !strings.ContainsRune("+-", u.op) {
		return fmt.Errorf("unexpected unary op %q", u.op)
	}
	return u.x.Check(vars)
}

//A binary represents a binary operator expression, e.g , x+y
type binary struct {
	op   rune //one of '+' ,'-'
	x, y Expr
}

// Check implements Expr.
func (b binary) Check(vars map[Var]bool) error {
	if !strings.ContainsRune("+-*/", b.op) {
		return fmt.Errorf("unexpected binary op %q", b.op)
	}
	if err := b.x.Check(vars); err != nil {
		return err
	}
	return b.y.Check(vars)
}

//A call represents a function call expression, e.g. sin(x)
type call struct {
	fn   string //one of "pow " , "sin", "sqrt"
	args []Expr
}

// Check implements Expr.
func (c call) Check(vars map[Var]bool) error {
	arity, ok := numParams[c.fn]
	if !ok {
		return fmt.Errorf("unknown function %q", c.fn)
	}
	if len(c.args) != arity {
		return fmt.Errorf("call to %s has %d args, want %d",
			c.fn, len(c.args), arity)
	}
	for _, arg := range c.args {
		if err := arg.Check(vars); err != nil {
			return err
		}
	}
	return nil
}

type Env map[Var]float64

//An Expr is an arithmetic express
type Expr interface {
	//Eval returns the value of this Expr in the environment env.
	Eval(env Env) float64
	//Check reports errors in this Expr and adds its Vars to the set.
	Check(vars map[Var]bool) error
}

func (v Var) Eval(env Env) float64 {
	return env[v]
}

func (l literal) Eval(_ Env) float64 {
	return float64(l)
}

func (u unary) Eval(env Env) float64 {
	switch u.op {
	case '+': //go语言中case不会穿透，除非使用fallthrough关键字
		return +u.x.Eval(env)
	case '-':
		return -u.x.Eval(env)
	}
	panic(fmt.Sprintf("unsupported unary operator: %q\n", u.op))
}

func (b binary) Eval(env Env) float64 {
	switch b.op {
	case '+':
		return b.x.Eval(env) + b.y.Eval(env)
	case '-':
		return b.x.Eval(env) - b.y.Eval(env)
	case '*':
		return b.x.Eval(env) * b.y.Eval(env)
	case '/':
		return b.x.Eval(env) / b.y.Eval(env)
	}
	panic(fmt.Sprintf("unsupported binary operator: %q", b.op))
}

func (c call) Eval(env Env) float64 {
	switch c.fn {
	case "pow":
		return math.Pow(c.args[0].Eval(env), c.args[1].Eval(env))
	case "sin":
		return math.Sin(c.args[0].Eval(env))
	case "sqrt":
		return math.Sqrt(c.args[0].Eval(env))
	}
	panic(fmt.Sprintf("unsupported function call: %s", c.fn))
}

func TestEval(t *testing.T) {
	tests := []struct {
		expr string
		env  Env
		want string
	}{
		//三个表达式和三个环境变量
		{"sqrt(A / pi)", Env{"A": 87616, "pi": math.Pi}, "167"},
		{"pow(x, 3) + pow(y, 3)", Env{"x": 12, "y": 1}, "1729"},
		{"pow(x, 3) + pow(y, 3)", Env{"x": 9, "y": 10}, "1729"},
		{"5 / 9 * (F - 32)", Env{"F": -40}, "-40"},
		{"5 / 9 * (F - 32)", Env{"F": 32}, "0"},
		{"5 / 9 * (F - 32)", Env{"F": 212}, "100"},
	}
	var prevExpr string //字符串的零值。nil
	for _, test := range tests {
		//Print expr only when it changes.
		if test.expr != prevExpr {
			fmt.Printf("\n%s\n", test.expr)
			prevExpr = test.expr
		}
		expr, err := Parse(test.expr)
		if err != nil {
			t.Error(err) //parse error
			continue
		}
		got := fmt.Sprintf("%.6g", expr.Eval(test.env))
		fmt.Printf("\t%v => %s\n", test.env, got)
		if got != test.want {
			t.Errorf("%s.Eval() in %v = %q, want %q\n",
				test.expr, test.env, got, test.want)
		}
	}
}

var numParams = map[string]int{"pow": 2, "sin": 1, "sqrt": 1}
