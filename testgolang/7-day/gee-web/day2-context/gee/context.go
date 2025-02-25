//实现上下文context
/*
实现Context的必要性：
1.
对于web服务来说，无非是根据请求*http.Request，构造http.ResponseWriter。但是这两个对象提供的接口粒度太细了。
比如：要构造一个完整的响应，需要考虑消息头(Header)和消息体(Body)，而Header包含了状态码(StatusCode)，消息
类型(ContentType)等几乎每次请求都需要设置的信息。因此，如果不进行有效的封装，那么框架的用户将需要写大量
重复，繁杂的代码，而且容易出错，有遗漏。因此，针对此场景，能够高效的构造出HTTP响应式一个好框架必须考虑的点
2.
对于web框架来说，还要支持额外的功能。例如，将来解析动态路由/hello/:name，参数:name的值放在哪里？再比如，框架
需要支持中间件，那中间件产生的信息放在哪里?Context随着每一个请求的出现而产生，请求的结束而销毁，和当前请求强
相关的信息都有Context承载，因此，设计Context结构，扩展性和复杂性留在了内部，而对外简化了接口。路由的处理函数
，以及将要实现的中间件，参数都同意使用了Context实例，Context应该包含一次会话的所有需要
*/
package gee

import (
	"encoding/json"
	"fmt"
	"net/http"
)

type H map[string]interface{} //键是string类型，值是空接口类型

//导出变量
type Context struct {
	//origin objects
	Writer http.ResponseWriter
	Req    *http.Request
	// request info
	Path   string
	Method string
	//response info
	StatusCode int
}

func newContext(w http.ResponseWriter, req *http.Request) *Context {
	return &Context{
		Writer: w,
		Req:    req,
		Path:   req.URL.Path,
		Method: req.Method, //GET or POST ??
	}
}

//方法
func (c *Context) PostForm(key string) string {
	//FormValue returns the first value for the named component of the query
	return c.Req.FormValue(key)
}

func (c *Context) Query(key string) string {
	return c.Req.URL.Query().Get(key)
}

func (c *Context) Status(code int) {
	c.StatusCode = code
	//WriteHeader sends an HTTP response header with the provided status code
	c.Writer.WriteHeader(code)
}

func (c *Context) SetHeader(key string, value string) {
	//Header returns the header map that will be sent by WriteHeader.
	// The Header map also is the mechanism with which Handlers can set HTTP trailers.
	c.Writer.Header().Set(key, value)
}

func (c *Context) String(code int, format string, values ...interface{}) {
	c.SetHeader("Content-Type", "text/plain")
	c.Status(code)
	//Write writes the data to the connection as part of an HTTP reply.
	//Sprintf formats according to a format specifier and returns the resulting string
	c.Writer.Write([]byte(fmt.Sprintf(format, values...)))
}

func (c *Context) JSON(code int, obj interface{}) {
	c.SetHeader("Content-Type", "application/json")
	c.Status(code)
	encoder := json.NewEncoder(c.Writer)
	if err := encoder.Encode(obj); err != nil {
		http.Error(c.Writer, err.Error(), 500)
	}
}

func (c *Context) Data(code int, data []byte) {
	c.Status(code)
	c.Writer.Write(data)
}

func (c *Context) HTML(code int, html string) {
	c.SetHeader("Content-Type", "text/html")
	c.Status(code)
	c.Writer.Write([]byte(html))
}
