#include<opencv2/opencv.hpp>
#include<opencv2/highgui.hpp>
#include<iostream>

using namespace std;
using namespace cv;

cv::Mat src;

int main()
{
    src = imread("/home/chenchao/code/testopencv/test1/img/catwoman.png");//这里是你的图片
    if(src.empty())
    {
        std::cout << "没有读取到图像" << endl;
        return -1;
    }
    imshow("My Image", src);  // 第一个参数是窗口名称
    waitKey(0);
    return 0;
}

