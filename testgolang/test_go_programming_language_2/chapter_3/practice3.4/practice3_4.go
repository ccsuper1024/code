/*
参考1.7节Lissajous例子的函数，构造一个服务器，用于计算函数曲面，然后返回SVG数据给客户端，服务器必须设置Content-Type头部
w.Header().Set("Conten-Type","image/svg+xml")
*/
package main
