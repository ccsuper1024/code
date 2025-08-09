/**
 * 不依赖Linux中的图形服务器，直接访问设备
 * 使用C语言直接访问/dev/fb0(framebuffer设备)
 * 获取屏幕分辨率、颜色深度
 * 在屏幕上绘制一个红色矩形
 */
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <linux/fb.h>
#include <sys/ioctl.h>
#include <sys/mman.h>
#include <unistd.h>
#include <string.h>
#include <stdint.h>
typedef enum
{
    INVALID_VALUE,
}RETURN_VALUE;
int main(void)
{
    const char* fb_path = "/dev/fb0";

    int fb = open(fb_path, O_RDWR);         //可读可写打开虚拟设备
    if(fb < 0)
    {
        perror("open fb");
        return -1;
    }

    //获取frameBuffer信息
    struct fb_var_screeninfo vinfo;
    struct fb_fix_screeninfo finfo;

    ioctl(fb, FBIOGET_FSCREENINFO, &finfo);
    ioctl(fb, FBIOGET_VSCREENINFO, &vinfo);

    int width = vinfo.xres;
    int height = vinfo.yres;
    int bpp = vinfo.bits_per_pixel;
    int line_len = finfo.line_length;

    printf("Resolution:%dx%d, %d bpp\n", width, height, bpp);

    long screensize = line_len * height;
    //将fb指向的显存的物理内存映射到fbp指针指向的位置
    uint8_t* fbp = (uint8_t*) mmap(0, screensize, PROT_READ|PROT_WRITE, MAP_SHARED,fb, 0);
    if(fbp == MAP_FAILED)
    {
        perror("mmap");
        close(fb);
        return -1;
    }

    //画一个矩形(左上角100x100,大小200x100)
    int x0 = 100, y0 = 100, w= 200, h = 100;
    for(int y = y0; y < y0 + h; y++)
    {
        for(int x = x0; x < x0 + w; x++)
        {
            long location = y * line_len + x * (bpp / 8);

            if(bpp == 32)
            {
                *(fbp + location + 0 ) = 0x00;      //B
                *(fbp + location + 1 ) = 0x00;      //G
                *(fbp + location + 2 ) = 0xFF;      //R
                *(fbp + location + 3 ) = 0x00;      //A
            }
            else if (bpp == 16)
            {
                //RGB565
                unsigned short color = (31 << 11);  //红色 整型值32位，其值为31，左移11位
                *((unsigned short*)(fbp + location)) = color;
            }
        }
    }

    sleep(5);

    munmap(fbp, screensize);
    close(fb);

    return 0;
}

