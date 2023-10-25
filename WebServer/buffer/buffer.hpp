/*
 * @file buffer.cpp 
 * @brief 这个缓存时可以在多线程下的循环队列式的缓存 
 */
#ifndef BUFFER_HPP
#define BUFFER_HPP
#include <cstring>  //perror
#include <iostream>
#include <unistd.h> //write
#include <sys/uio.h> //readv :从文件描述符中读到多个缓存区
#include <vector>
#include <atomic>   //原子类型，无锁编程
#include <assert.h>
using std::vector;
using std::string;
using std::atomic;

class Buffer
{
public:
    Buffer(int initBufferSize = 1024);
    ~Buffer();

    size_t WritableBytes() const;
    size_t ReadableBytes() const;
    //可预置的
    size_t PrependableBytes() const;

    const char* Peek() const;
    void EnsureWritable(size_t len);
    void HashWritten(size_t len);

    void Retrieve(size_t len);
    void RetrieveUntil(const char* end);

    void RetrieveAll();
    string RetrieveAllToStr();

    const char* BeginWriteConst() const;
    char* BeginWrite();

    void Append(const string& str);
    void Append(const char* str,size_t len);
    void Append(const void* data,size_t len);
    void Append(const Buffer& buff);

    ssize_t ReadFd(int fd,int* Errno);
    ssize_t WriteFd(int fd, int* Errno);
private:
    char* BeginPtr();
    //重载函数，成员函数的第一个参数是隐含的this指针。下面的函数被const修饰了this指针。是const成员函数
    //可以被const对象调用
    const char* BeginPtr()const;
    void MakeSpace(size_t len);

    vector<char> _buffer;
    atomic<size_t> _readPos;
    atomic<size_t> _writePos;
};

#endif  //BUFFER_HPP
