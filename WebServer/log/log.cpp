/**
 * @file 
 * @brief 
 */
//最简单例模式的创建
//1.将构造函数私有化，使得外部无法构造对象
//2.定义一个静态函数getInstance();
//3.在静态函数中构造一个静态对象，再返回此静态对象。局部的静态对象是存储在全局静态区，
//即数据段中的。由于存储的是一整个对象，有时静态对象太大会过多的占用数据段空间。所以
//大多数时候应该采用另一种单例模式的构建犯法
//1.定义一个静态指针，此指针指向当前类类型
//2.在当前文件的类外初始化静态指针。静态变量应该在头文件中初始化，在实现文件中初始化会造成
//重复定义
//3.构造函数私有化
//4.定义静态函数getInstance()和destroy()。在getInstance中先判断静态指针是否为空，如果为空
//则new constructor() 。在返回getInstance()的返回类型
#include "log.hpp"
#include <iostream>


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
//静态函数不属于类，是全局的，这才能保证Log类对象的单例，只会被创建一次，后续再调用此函数
//也只会返回第一次初始化时的对象的指针
Log* Log::Instance() 
{
    static Log inst;
    return &inst;
}
void Log::FlushLogThread() 
{
    Log::Instance()->AsyncWrite();
}

void Log::init(int level,const char* path ,const char* suffix ,
          int maxQueueCapacity ) 
{
    _isOpen = true;
    _level = level;
    if(maxQueueCapacity > 0){
        _isAsync = true;
        if(!_deque){
            unique_ptr<BlockQueue<string>> newDeque(new BlockQueue<string>);
            _deque = std::move(newDeque);

            std::unique_ptr<std::thread> newThread(new thread(::FlushLogThread));
            _writeThread = std::move(newThread);
        }
    }else{
        _isAsync = false;
    }

    _lineCount = 0;
    //获取当前时间
    time_t timer = time(nullptr);  
    //转换为本地时间
    struct tm* sysTime = localtime(&timer);
    struct tm t = *sysTime;
    _path = path;
    _suffix = suffix;
    char filename[LOG_NAME_LEN] = {0};
    snprintf(filename,LOG_NAME_LEN-1,"%s/%04d_%02d_%02d%s",_path,t.tm_year + 1900,
             t.tm_mon + 1,t.tm_mday,_suffix);
    _toDay = t.tm_mday; 
    {
        std::lock_guard<mutex> lock(_mtx);
        _buff.RetrievAll();
        if(_fp){
            flush();
            fclose(_fp);
        }

        _fp = fopen(filename,"a");
        if(_fp == nullptr){
            mkdir(_path,0777);
            _fp = fopen(filename,"a");
        }
        assert(_fp != nullptr);
    }
}

void Log::write(int level,const char* format,...) 
{
    struct timeval now = {0,0};
    gettimeofday(&now,nullptr);
    time_t tSec = now.tv_sec;
    struct tm* sysTime = localtime(&tSec);
    struct tm t = *sysTime;
    va_list vaList;

    //日志日志，日志行数
    if(_toDay != t.tm_mday || (_lineCount && (_lineCount % MAX_LINES == 0))){
        //定义一把独占式的锁
        std::unique_lock<mutex> lock(_mtx);
        //解锁
        lock.unlock();
        char newFile[LOG_NAME_LEN];
        char tail[36] = {0};
        //拼接字符串 年月日
        snprintf(tail,36,"%04d_%02d_%02d",t.tm_year + 1900,t.tm_mon + 1,t.tm_mday);

        if(_toDay != t.tm_mday)
        {
            snprintf(newFile,LOG_NAME_LEN -72,"%s/%s%s",_path,tail,_suffix);
            _toDay = t.tm_mday;
            _lineCount = 0;
        }else{
            snprintf(newFile,LOG_NAME_LEN-72,"%s/%s-%d%s",_path,tail,(_lineCount / MAX_LINES),
                     _suffix);
        }
        //加锁
        lock.lock();
        flush();
        fclose(_fp);
        _fp = fopen(newFile,"a");
        assert(_fp != nullptr);
    }

    {
        std::unique_lock<mutex> lock(_mtx);
        _lineCount++;
        int n = snprintf(_buff.BeginWrite(),128,"%d-%02d-%02d %02d:%02d:%02d.%06ld",
                         t.tm_year+1900,t.tm_mon+1,t.tm_mday,t.tm_hour,t.tm_min,t.tm_sec,
                         now.tv_usec);
        _buff.HashWritten(n);
        AppendLogLevelTitle(level);

        //用format这个最后一个固定参数的位置来得到可变参数列表的起始地址，并赋给vaList
        va_start(vaList,format);
        //将可变数量的参数格式化输出到一个字符串中.将vaList所指的可变参数以format的格式格式化到_buff中
        int m = vsnprintf(_buff.BeginWrite(),_buff.WritabelBytes(),format,vaList);
        va_end(vaList);
        
        _buff.HashWritten(m);
        _buff.Append("\n\0",2);

        if(_isAsync && _deque && !_deque->full()){
            _deque->push_back(_buff.RetrievAllToStr());
        }else{
            fpus(_buff.Peek(),_fp);
        }
        _buff.RetrievAll();
    }

}
void Log::flush() 
{
    if(_isAsync){
        _deque->flush();
    }
    fflush(_fp);
}

int Log::GetLevel() 
{
    std::lock_guard<mutex> lock(_mtx);
    return _level;
}
void Log::SetLevel(int level) 
{
    std::lock_guard<mutex> lock(_mtx);
    _level = level;
}
 Log::Log() 
{
    _lineCount = 0;
    _isAsync = false;
    _writeThread = nullptr;
    _deque = nullptr;
    _toDay = 0;
    _fp = nullptr;
}
 Log::~Log() 
{
    if(_writeThread && _writeThread->joinable()){ //线程是汇合的
        while(!_deque->empty()){
            _deque->flush();
        }
        _deque->close();
        _writeThread->join();   //阻塞等待线程结束
    }
    if(_fp){
        std::lock_guard<mutex> lock(_mtx);
        flush();
        fclose(_fp);
    }
}
//追加日志等级
void Log::AppendLogLevelTitle(int level) 
{
    switch(level){
    case 0:{
               _buff.Append("[debug]",9);
           }
    case 1:{
               _buff.Append("[info]",9);
           }
    case 2:{
               _buff.Append("[warn]",9);
           }
    case 3:{
               _buff.Append("[error]",9);
           }
    default:{
               _buff.Append("[info]",9);
           }
    }
}
//异步写
void Log::AsyncWrite() 
{
    string str = "";
    while(_deque->pop(str)){    //双端队列中弹出一个string对象到str中，则加锁写入文件中
        std::lock_guard<mutex> lock(_mtx);
        fputs(str.c_str(),_fp);
    }
}
