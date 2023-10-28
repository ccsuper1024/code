/* @file log.cpp
 * @brief log
 */
#ifndef LOG_HPP
#define LOG_HPP
#include "../buffer/buffer.hpp"
#include "./blockqueue.hpp"
#include <string.h>
#include <stdarg.h>     //va_list va_start() va_end()
#include <cassert>
#include <sys/stat.h>
#include <sys/time.h>
#include <mutex>
#include <string>
#include <thread>
#include <memory>

using std::unique_ptr;
using std::string;
using std::thread;
using std::mutex;

//以单例模式实现Log类,在程序中只需要初始化一次Log类对象即可
class Log
{
public:
    //静态函数不属于类，是全局的，这才能保证Log类对象的单例，只会被创建一次，后续再调用此函数
    //也只会返回第一次初始化时的对象的指针
    static Log* Instance();
    static void FlushLogThread();
   
    void init(int level = 1,const char* path = "./log",const char* suffix = ".log",
                            int maxQueueCapacity = 1024);

    void write(int level,const char* format,...);
    void flush();

    int GetLevel();
    void SetLevel(int level);
    bool IsOpen(){
        return _isOpen;
    }
private:
    Log();
    virtual ~Log();
    //追加日志等级
    void AppendLogLevelTitle(int level);
    //异步写
    void AsyncWrite();
private:
    //静态数据成员，不属于类对象，是全局的
    //常量整型可以在类内初始化，其他类型不可以。只有static constexpr 修饰的类型才是真的常量()
    static const int LOG_PATH_LEN = 256;
    static const int LOG_NAME_LEN = 256;
    static const int MAX_LINES = 50000;

    const char* _path;
    const char* _suffix;

    int _MAX_LNES;

    int _lineCount;
    int _toDay;

    bool _isOpen;

    Buffer _buff;
    int _level;
    bool _isAsync;

    FILE* _fp;
    unique_ptr<BlockQueue<string>> _deque;
    unique_ptr<thread> _writeThread;
    mutex _mtx;
    static Log* pInstance;
};
//饱汉模式
Log* Log::pInstance = nullptr;
#define LOG_BASE(level,format,...)  \
    do{                             \
        Log* log = Log::Instance(); \
        if(log->IsOpen() && log->GetLevel() <= level){           \
            log->write(level,format,##__VA_ARGS__);             \
            log->flush();                                       \
        }                                                       \
    }while(0);

#define LOG_DEBUG(format,...) do { LOG_BASE(0,format,##__VA_ARGS__)}while(0);
#define LOG_INFO(format,...) do { LOG_BASE(1,format,##__VA_ARGS__)}while(0);
#define LOG_WARN(format,...) do { LOG_BASE(2,format,##__VA_ARGS__)}while(0);
#define LOG_ERROR(format,...) do { LOG_BASE(3,format,##__VA_ARGS__)}while(0);

#endif  //LOG_H
