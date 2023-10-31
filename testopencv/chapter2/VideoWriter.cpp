/**
 * @file VideoWriter.cpp 
 * @brief 使用VideoWriter类可以读写摄像头和视频流，并持久化保存
 */
#include <iostream>
#include <opencv2/opencv.hpp>
#include <opencv2/video.hpp>
#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>
#include <string>

using std::string;
using namespace cv;
using std::cout;
using std::cin;
using std::endl;
using std::cerr;


/**
 * @brief 
 * @param 
 * @param 
 * @return 
 */
int main()
{
    Mat img;
    //使用某个摄像头 
    VideoCapture video(0);  
                            
    //读取视频
    /* VideoCapture videoStream; */
    /* videoStream.open("cup.mp4"); */

    //判断是否调用成功
    if(!video.isOpened()){
        cout << "打开摄像头失败，请确认摄像头是否安装成功"<< endl;
        return -1;
    }
    
    //从视频流中获取逐帧图像
    video >> img;   
    //检查是否失败
    if(img.empty()){
        cout << "没有获取图像" << endl;
        return -1;
    }
    //判断相机类型是否为彩色
    bool isColor = (img.type() == CV_8UC3);

    //初始化 VideroWriter类对象
    //保存视频
    VideoWriter vwriter;
    //选择图片编码格式
    int codec = VideoWriter::fourcc('M','J','P','G');

    //设置视频帧率，人眼所看到的视频，最低帧率为24
    double fps = 25.0;
    //保存文件的名称
    string filename = "live.avi";
    //创建保存视频文件的视频流
    vwriter.open(filename,codec,fps,img.size(),isColor);
    if(!vwriter.isOpened()){
        cout << "打开视频文件失败，..." << endl;
        return -1;
    }
    //--- GRAB AND WRITE LOOP
    cout << "Writing videofile: " << filename << endl
         << "Press any key to terminate" << endl;
    while(1){
        //检测是否执行完毕
        if(!video.read(img)) //判断是否继续从摄像头或者视频中读出一帧图像
        {
            cout << "摄像头断开连接或者视频读取完成"  << endl;
            break;  //跳出while/for/switch循环
        }
        //把图像写入视频流
        vwriter.write(img);
        //显示图像
        imshow("Live",img);

        waitKey(0);
    }

    //退出程序时自动关闭视频流
    video.release();
    vwriter.release();
    return 0;
}


