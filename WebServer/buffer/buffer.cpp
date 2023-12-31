/**
 * @file buffer.cpp 
 * @brief implementation of buffer
 */
#include "buffer.hpp"

using std::cout; 
using std::cin; 
using std::endl; 
using std::cerr; 

/**
 * @brief 
 * @param 
 * @return 
 */
Buffer::Buffer(int initBufferSize) 
:_buffer(initBufferSize)
,_readPos(0)
,_writePos(0)
{
    cout << "Buffer()" << endl;
}
/**
 * @brief 
 * @param 
 * @return 
 */
Buffer::~Buffer()
{
    cout << "~Buffer()" << endl;
}

/**
 * @brief 
 * @param 
 * @return 
 */
//const 成员函数
size_t Buffer::WritableBytes() const 
{
    return _writePos - _readPos;
}
/**
 * @brief 
 * @param 
 * @return 
 */
size_t Buffer::ReadableBytes() const 
{
    return _buffer.size() - _writePos;
}
/**
 * @brief 
 * @param 
 * @return 
 */
size_t Buffer::PrependableBytes() const //可预置的字节数
{
    return _readPos;
}
/**
 * @brief 
 * @param 
 * @return 
 */
const char* Buffer::Peek() const 
{
    return BeginPtr() + _readPos;
}
/**
 * @brief 
 * @param 
 * @return 
 */
void Buffer::EnsureWritable(size_t len) 
{
    if(WritableBytes() < len){
        MakeSpace(len);
    }
    assert(WritableBytes() >= len);
}
/**
 * @brief 
 * @param 
 * @return 
 */
void Buffer::HashWritten(size_t len) 
{
    _writePos += len;
}

/**
 * @brief 
 * @param 
 * @return 
 */
void Buffer::Retrieve(size_t len) 
{
    //断言，如果len > ReadableBytes() 就会中断程序，类似执行了exit();
    assert(len <= ReadableBytes());
    _readPos += len;
}
/**
 * @brief 
 * @param 
 * @return 
 */
void Buffer::RetrieveUntil(const char* end) 
{
    assert(Peek() <= end);
    Retrieve(end - Peek());
}

/**
 * @brief 
 * @param 
 * @return 
 */
void Buffer::RetrieveAll() 
{
    bzero(&_buffer[0],_buffer.size());
    _readPos = 0;
    _writePos = 0;
}
/**
 * @brief 
 * @param 
 * @return 
 */
string Buffer::RetrieveAllToStr() 
{
    string str(Peek(),ReadableBytes());
    RetrieveAll();
    return str;
}

/**
 * @brief 
 * @param 
 * @return 
 */
const char* Buffer::BeginWriteConst() const 
{
    return BeginPtr() + _writePos;
}
/**
 * @brief 
 * @param 
 * @return 
 */
char* Buffer::BeginWrite() 
{
    return BeginPtr() + _writePos;
}

/**
 * @brief 
 * @param 
 * @return 
 */
void Buffer::Append(const string& str) 
{
    Append(str.data(),str.length());
}
/**
 * @brief 
 * @param 
 * @return 
 */
void Buffer::Append(const char* str,size_t len) 
{
    assert(str);    //检查str是否为nullptr
    EnsureWritable(len);
    std::copy(str,str+len,BeginWrite());
    HashWritten(len);
}
/**
 * @brief 
 * @param 
 * @return 
 */
void Buffer::Append(const void* data,size_t len) 
{
    assert(data);
    Append(static_cast<const char*>(data),len);
}
/**
 * @brief 
 * @param 
 * @return 
 */
void Buffer::Append(const Buffer& buff) 
{
    Append(buff.Peek(),buff.ReadableBytes());
}

/**
 * @brief 
 * @param 
 * @return 
 */
ssize_t Buffer::ReadFd(int fd,int* Errno) 
{
    char buff[65535];
    struct iovec iov[2];
    const size_t writable = WritableBytes();
    /* 分散读，保证数据全部读完 */
    iov[0].iov_base = BeginPtr() + _writePos;
    iov[0].iov_len = writable;
    iov[1].iov_base = buff;
    iov[1].iov_len = sizeof(buff);

    const ssize_t len = readv(fd,iov,2);
    if(len < 0){
        *Errno = errno;
    }else if(static_cast<size_t>(len) <= writable){
        _writePos += len;
    }else{
        _writePos = _buffer.size();
        Append(buff,len - writable);
    }
    return len;
}
/**
 * @brief 
 * @param 
 * @return 
 */
ssize_t Buffer::WriteFd(int fd, int* Errno) 
{
    ssize_t readSize = ReadableBytes();
    ssize_t len = write(fd,Peek(),readSize);
    if(len < 0){
        *Errno = errno;
        return len;
    }
    _readPos += len;
    return len;
}
/**
 * @brief 
 * @param 
 * @return 
 */
char* Buffer::BeginPtr() 
{
    return &*_buffer.begin();
}
/**
 * @brief 
 * @param 
 * @return 
 */
//重载函数，成员函数的第一个参数是隐含的this指针。下面的函数被const修饰了this指针。是const成员函数
//可以被const对象调用
const char* Buffer::BeginPtr()const 
{
    return &*_buffer.cbegin();

}
/**
 * @brief 
 * @param 
 * @return 
 */
void Buffer::MakeSpace(size_t len) 
{
    if(WritableBytes() + PrependableBytes() < len){
        _buffer.resize(_writePos + len + 1);
    }else{
        size_t readable = ReadableBytes();
        std::copy(BeginPtr() + _readPos ,BeginPtr() + _writePos , BeginPtr());
        _readPos = 0;
        _writePos = _readPos + readable;
        assert(readable == ReadableBytes());
    }
}
