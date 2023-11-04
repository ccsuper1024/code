/**
 * @file 
 * @brief 
 */
#include <iostream>
#include <workflow/WFHttpServer.h>/*无名称空间*/
#include <fstream>      //文件对象

using std::ifstream;
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
    //读取文件中的内容
    ifstream infile("./test.html");
    if(!infile){
        cerr << "check you input file" << endl; 
    }
     //定位到文件的最末尾
    infile.seekg(0,std::ios::end);  
    //获取文件的大小
    int size = infile.tellg();
    //将文件指针重新定位到开头
    infile.seekg(0,std::ios::beg);
    //动态分配一个文件大小的基本元素为char的内存
    char* buffer = new char[size];
    //一次性从磁盘读取整个文件的内容到内存
    infile.read(buffer,size);

    WFHttpServer server([buffer](WFHttpTask *task) {
        task->get_resp()->append_output_body(buffer);
    });

    if (server.start(8888) == 0) {  // start server on port 8888
        getchar(); // press "Enter" to end.
        server.stop();
    }
    return 0;
}


