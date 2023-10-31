/**
 * @file 
 * @brief 
 */
#include "buffer.hpp"
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <iostream>
#include <string>
#include <fstream>

using std::ifstream;
using std::ofstream;
using std::string;
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
    const int buffsize = 1024;
    Buffer buff(buffsize);

    cout << "buff's readableBytes is " << buff.ReadableBytes();
    cout << "buff's writableBytes is " << buff.WritableBytes();

    string str = {"chenchao is stupid man and he is lose his mind"};
    buff.Append(std::move(str));
    cout << "Now, buff's readableBytes is " << buff.ReadableBytes();
    cout << "Now, buff's writableBytes is " << buff.WritableBytes();

    //以可读打开文件，进行分散读
    int readfd =  open("./test.txt",O_RDONLY);
    //以只写方式打开文件
    int writefd = open("./write.txt",O_WRONLY);
    //写入另一个文件，由于buff对象底层实现是动态数组--容器vector。它会自动扩容
    int saveError;
    ssize_t ret = buff.ReadFd(readfd,&saveError);
    perror(strerror(saveError));

    buff.WriteFd(writefd,&saveError);
    perror(strerror(saveError));

    //输出缓存中的字符数据,并清空缓存
    auto out = buff.RetrieveAllToStr();
    cout << out << endl;

    return 0;
}


