//Lissajous generates GIF animations of random Lissajous figures
package main

import (
	"image"
	"image/color"
	"image/gif"
	"io"
	"math"
	"math/rand"
	"os"
	"time"
)

var palette = []color.Color{color.White, color.Black}

const (
	whiteIndex = 0 //first color in palette
	blackIndex = 1 //next color in palette
)

func lissajous(out io.Writer) {
	const (
		cycles  = 5     //number of complete x oscillator revolutions(震荡期转数)
		res     = 0.001 //angular resolution
		size    = 100   //image canvas covers [-size +size]
		nframes = 64    //number of animation frames
		delay   = 8     //delay between frames in 10ms units
	)

	freq := rand.Float64() * 3.0        //relative frequency of y oscillator
	anim := gif.GIF{LoopCount: nframes} //复合声明，生成一个结构体，并且内部变量LoopCount字段被设置为nframes
	phase := 0.0                        //phase difference
	for i := 0; i < nframes; i++ {
		//func image.NewPaletted(r image.Rectangle, p color.Palette) *image.Paletted
		rect := image.Rect(0, 0, 2*size+1, 2*size+1) //201 x 201
		//func image.NewPaletted(r image.Rectangle, p color.Palette) *image.Paletted
		img := image.NewPaletted(rect, palette)
		for t := 0.0; t < cycles*2*math.Pi; /* 2πR*/ t += res {
			x := math.Sin(t)
			y := math.Sin(t*freq + phase)
			img.SetColorIndex(size+int(x*size+0.5), size+int(y*size+0.5),
				blackIndex)
		}
		phase += 0.1
		anim.Delay = append(anim.Delay, delay)
		anim.Image = append(anim.Image, img)
	}
	gif.EncodeAll(out, &anim)
}

func main() {
	//The sequence of images is deterministic(确定性的) unless we need
	//the pseudo-random(伪随机) number generator using the current time.
	//Thanks to Randall McPherson for pointing out the omission(遗漏)
	rand.Seed(time.Now().UTC().UnixNano())
	lissajous(os.Stdout)
}
