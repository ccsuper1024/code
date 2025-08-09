// Lissajous generates GIF animations of random Lissajous figures
package main

import (
	"image"
	"image/color"
	"image/gif"
	"io"
	"math"
	"math/rand"
)

// 全局变量，调色板，实现彩色调色板
var g_palette = []color.Color{color.White, color.Black,
	color.RGBA{0xff, 0x00, 0x00, 0xff}, //red
	color.RGBA{0x00, 0xff, 0x00, 0xff}, //green
	color.RGBA{0x00, 0x00, 0xff, 0xff}, //blue
	color.RGBA{0xff, 0xff, 0x00, 0xff}, //yellow
	color.RGBA{0xff, 0x00, 0xff, 0xff}, //purple
	color.RGBA{0x00, 0xff, 0xff, 0xff}, //blue-green
}

const (
	whiteIndex = 0 //first color in palette(调色板)
	blackIndex = 1 //此处的值和上面的全局变量中的颜色的位置对应
	greenIndex = 2
)

func Lissajous(out io.Writer) {

	//声明常量，const包裹变量名
	//以下参数控制动画的像精度，大小，长度等
	const (
		cycles  = 5     //number of complete x oscillator revolutions	正当其的完整震荡周期数
		res     = 0.001 //angular resolution							角都分辨率(控制点的密度)
		size    = 100   //image canvas covers [-size..+size]			图像尺寸，图像坐标范围是[-size, +size]
		nframes = 64    //number of animation frames					动画帧数
		delay   = 8     //delay between frames in 10ms units			每帧之间的间隔
	)

	freq := rand.Float64() * 3.0        //relative frequency of y oscillator y轴的频率
	anim := gif.GIF{LoopCount: nframes} //创建一个GIF数据结构，并将其中的LoopCount字段初始化为nframes常量
	phase := 0.0                        //phase difference	相位差
	for i := 0; i < nframes; i++ {
		rect := image.Rect(0, 0, 2*size+1, 2*size+1) //生成一个矩形图形，前两个值构成矩形的左上角，后两个值构成矩形的右下角
		img := image.NewPaletted(rect, g_palette)
		for t := 0.0; t < cycles*2*math.Pi; t += res {
			x := math.Sin(t)
			y := math.Sin(t*freq + phase)

			colorIndex := uint8(i % 8)

			//在图像上设置指定像素为黑色,blackIndex为1,就colorIndex与i关联实现彩色图像
			img.SetColorIndex(size+int(x*size+0.5), size+int(y*size+0.5), colorIndex) //go没有隐式类型转换
		}
		//增加相位差，让每一帧图像稍微变化，实现动画效果
		phase += 0.1

		//将当前帧的延迟时间加入动画中
		anim.Delay = append(anim.Delay, delay)
		//将当前帧加入动画中
		anim.Image = append(anim.Image, img)
	}
	gif.EncodeAll(out, &anim) //NOTE:ignoring encoding errors
}
