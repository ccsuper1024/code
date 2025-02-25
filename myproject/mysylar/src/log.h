/**
 * @file log.h
 * @brief 日志模块封装
 * @author chenchao
 * @email 1024204974@qq.com
 * @date 20240814
 * @copyright
 */

#ifndef __MYSYLAR_LOG_H__
#define __MYSYLAR_LOG_H__
#include <cstdint>
#include <string>
#include <memory>
#include <list>
#include <sstream>
#include <fstream>
#include <vector>
#include <map>
#include <cstdarg>
#include "util.h"
#include "singleton.h"
#include "thread.h"

/**
 * @brief 使用流式方式将日志级别level的日志写入日志
*/
#define SYLAR_LOG_LEVEL(logger, level) \
    if(logger->getLevel() <= level) \
        sylar::LogEventWrap(sylar::LogEvent::ptr(new sylar::LogEvent(logger, level, \
                            __FILE__, __LINE__, 0, sylar::GetThreadId(),\
                            sylar::GetFiberId(), time(0), sylar::Thread::GetName()))).getSS()

/**
 * @brief 使用流式方式将日志级别DEBUG的日志写入 日志
 */
#define SYLAR_LOG_DEBUG(logger) SYLAR_LOG_LEVEL(logger, sylar::LogLevel::DEBUG)

/**
 * @brief 使用流式方式将日志级别INFO的日志写入 日志
 */
#define SYLAR_LOG_INFO(logger) SYLAR_LOG_LEVEL(logger, sylar::LogLevel::INFO)

/**
 * @brief 使用流式方式将日志级别WARN的日志写入 日志
 */
#define SYLAR_LOG_WARN(logger) SYLAR_LOG_LEVEL(logger, sylar::LogLevel::WARN)

/**
 * @brief 使用流式方式将日志级别ERROR的日志写入 日志
 */
#define SYLAR_LOG_ERROR(logger) SYLAR_LOG_LEVEL(logger, sylar::LogLevel::ERROR)

/**
 * @brief 使用流式方式将日志级别FATAL的日志写入 日志
 */
#define SYLAR_LOG_FATAL(logger) SYLAR_LOG_LEVEL(logger, sylar::LogLevel::FATAL)

/**
 * @brief 使用格式化方式将日志级别level的日志写入到logger
 * __VA_ARGS__是可变参数宏，用于替代...的位置
 */
#define SYLAR_LOG_FMT_LEVEL(logger, level, fmt, ...) \
            sylar::LogEventWrap(sylar::LogEvent::ptr(new sylar::LogEvent(logger, level, \
                                __FILE__, __LINE__, 0, sylar::GetThreadId(), \
                                sylar::GetFiberId(), time(NULL),\ 
                                sylar::Thread::GetName()))).getEvent()->format(fmt, __VA__ARGS__)

/**
 * @brief 使用格式化方式将日志级别DEBUG的日志写入到logger
 */
#define SYLAR_LOG_FMT_DEBUG(logger, fmt, ...) \
            SYLAR_LOG_FMT_LEVEL(logger, sylar::LogLevel::DEBUG,fmt, __VA_ARGS__)

/**
 * @brief 使用格式化方式将日志级别INFO的日志写入到logger
 */
#define SYLAR_LOG_FMT_DEBUG(logger, fmt, ...) \
            SYLAR_LOG_FMT_LEVEL(logger, sylar::LogLevel::INFO,fmt, __VA_ARGS__)

/**
 * @brief 使用格式化方式将日志级别WARN的日志写入到logger
 */
#define SYLAR_LOG_FMT_WARN(logger, fmt, ...) \
            SYLAR_LOG_FMT_LEVEL(logger, sylar::LogLevel::WARN,fmt, __VA_ARGS__)

/**
 * @brief 使用格式化方式将日志级别ERROR的日志写入到logger
 */
#define SYLAR_LOG_FMT_ERROR(logger, fmt, ...) \
            SYLAR_LOG_FMT_LEVEL(logger, sylar::LogLevel::ERROR,fmt, __VA_ARGS__)

/**
 * @brief 使用格式化方式将日志级别FATAL的日志写入到logger
 */
#define SYLAR_LOG_FMT_FATAL(logger, fmt, ...) \
            SYLAR_LOG_FMT_LEVEL(logger, sylar::LogLevel::FATAL,fmt, __VA_ARGS__)

/**
 * @brief 获取name的日志器
 */
#define SYLAR_LOG_NAME(name) sylar::LoggerMgr::GetInstance()->getLogger(name)
/**
 * @brief 获取主日志器
 */
#define SYLAR_LOG_ROOT() sylar::LoggerMgr::GetInstance()->getRoot() 

namespace sylar
{
    //前置声明
    class Logger;
    class LoggerManager;
    /**
     * @brief 日志级别
     */
    class LogLevel
    {
    public:
        /**
         * @brief 日志级别枚举
         * @note 嵌入枚举类
         */
        enum Level{             //非限定作用域枚举，枚举类中的枚举值和枚举类拥有同一作用域
            UNKNOWN = 0,        //未知级别
            DEBUG   = 1,        //DEBUG级别
            INFO    = 2,        //INFO级别
            WARN    = 3,        //WARN级别
            ERROR   = 4,        //ERROR级别
            FATAL   = 5         //FATAL级别
        };

        /**
         * @brief 将日志级别转成文本输出
         * @param[in] level 日志级别 
         */
        static const char *ToString(LogLevel::Level level); // 静态成员函数

        /**
         * @brief 将文本转换为日志级别 
         * @param[in] str 文本输出
         */
        static LogLevel::Level FromString(const std::string &str);

    };

    /**
     * @brief 日志事件
    */
    class LogEvent {
    public:
        using ptr = std::shared_ptr<LogEvent>;

        /**
         * @brief LogEvent的构造函数
         * @param[in] logger 日志器
         * @param[in] level 日志级别
         * @param[in] file  文件名
         * @param[in] line 行号
         * @param[in] elapse 程序启动依赖的耗时(毫秒)
         * @param[in] threadId 线程ID
         * @param[in] fiberId 协程ID
         * @param[in] time 日志事件(秒)
         * @param[in] threadName 线程名称
         */ 
        LogEvent(std::shared_ptr<Logger> logger, LogLevel::Level level,
                const char* fileName, int32_t line, uint32_t elapse,
                uint32_t threadId, uint32_t fiberId, uint64_t timeStamp,
                const std::string& threadName);
        /*其他编译器合成的函数也要注意，见Effective Modern C++*/

        /**
         * @brief 返回文件名
         * @note const成员函数
        */
        const char* getFile() const {return m_file; }

        /**
         * @brief 返回行号 
         * @note const成员函数
        */
        int32_t getLine() const { return m_line; }

        /**
         * @brief 返回耗时 
         * @note const成员函数
        */
        uint32_t getElapse() const { return m_elapse; }

        /**
         * @brief 返回线程ID
         * @note const成员函数
        */
        uint32_t getThreadId() const { return m_threadId; }
        
        /**
         * @brief 返回协程ID 
         * @note const成员函数
        */
        uint32_t getFiberId() const { return m_fiberId; }

        /**
         * @brief 返回时间
         * @note const成员函数
        */
        uint64_t getTimeStamp() const { return m_timeStamp; }

        /**
         * @brief 返回线程名称
         * @note const成员函数
        */
        const std::string& getThreadName() const { return m_threadName; }

        /**
         * @brief 返回日志内容
         * @note const成员函数 
        */
        std::string getContent() const { return m_ss.str(); }

        /**
         * @brief 返回日志器
         * @note 并不是从自身创建一个std::shared_ptr对象
         * @note const成员函数，const对象和非const对象都可使用，后置const修饰this指针指向对象
         */
        std::shared_ptr<Logger> getLogger() const { return m_logger; }

        /**
         * @brief 返回日志级别 
         * @note const成员函数
         */
        LogLevel::Level getLevel() const { return m_level; }

        /**
         * @brief 返回日志内容字符串流 
         * @note 流对象不可复制
         */
        std::stringstream& getSS() { return m_ss; }

        /**
         * @brief 格式化写入日志内容 
         */
        void format(const char* fmt, va_list al);
        /**
         * @brief 格式化写入日志内容 
         */
        void format(const char* fmt, ...);

    private:
        //文件名
        const char* m_file = nullptr;
        //行号
        int32_t m_line = 0;
        //程序启动开始到现在的毫秒数
        int32_t m_elapse = 0;
        //线程ID
        uint32_t m_threadId = 0;
        //协程ID
        uint32_t m_fiberId = 0;
        //时间戳
        uint64_t m_timeStamp = 0;
        //线程名称
        std::string m_threadName;
        //日志内容流
        std::stringstream m_ss;
        //日志器
        std::shared_ptr<Logger> m_logger;       //智能指针指向不完整类型
        //日志等级
        LogLevel::Level m_level;
    };

    /**
     * @brief 日志事件包装器
    */
    class LogEventWrap{
    public:
        /**
         * @brief 构造函数
         * @param[in] e 日志事件
         */
        LogEventWrap(LogEvent::ptr e);

        /**
         * @brief 析构函数
         */
        ~LogEventWrap();

        /* 编译器合成复制语义，不会合成默认构造函数 */

        /**
         * @brief 获取日志事件
         * @note const成员函数，可被const对象和非const对象使用，const修饰的是this指向的对象
         */
        LogEvent::ptr getEvent() const { return m_event; }

        /**
         * @brief 获取日志内容流
         * @note  流对象不可复制
         */
        std::stringstream& getSS();
    private:
        /**
         * @brief 日志事件
         */
        LogEvent::ptr m_event;
    };

    /**
     * @brief 日志格式化
     */
    class LogFormatter {
    public:
        using ptr = std::shared_ptr<LogFormatter>;
        /**
         * @brief 构造函数
         * @param[in] pattern 格式模板
         * @details
         *      %m 消息
         *      %p 日志级别
         *      %r 累积毫秒数
         *      %c 日志名称
         *      %t 线程ID
         *      %n 换行
         *      %d 时间
         *      %f 文件名
         *      %l 行号
         *      %T 制表符
         *      %F 协程ID
         *      %N 线程名称
         * 
         * 默认格式："%d{%Y-%m-%d %H:%M:%S}%T%t%T%N%T%F[%p]%T[%c]%T%f:%l%T%m%n"
         */
        LogFormatter(const std::string& pattern);

        /**
         * @brief 返回格式化日志文本
         * @param[in] logger 日志器
         * @param[in] level 日志级别
         * @param[in] event 日志事件
         */
        std::string format(std::shared_ptr<Logger> logger, LogLevel::Level level,
                            LogEvent::ptr event);
        
        /**
         * @brief 返回格式化日志文本并写入流对象
         */
        std::ostream& format(std::ostream& ofs, std::shared_ptr<Logger> logger, 
                                LogLevel::Level level, LogEvent::ptr event);
         
    public:
        /**
         * @brief 日志内容格式化
         * @note 嵌入类
         */
        class FormatItem{
        public:
            using ptr = std::shared_ptr<FormatItem>; 

            /**
             * @brief 析构函数
             */
            virtual ~FormatItem() {}

            /* 注意编译器自动合成复制语义和默认构造函数 */
            /**
             * @brief 格式化日志到流对象
             * @param[in,out] os 日志输出流
             * @param[in] logger 日志器
             * @param[in] 日志等级
             * @param[in] event 日志事件
             * @note 纯虚函数，抽象基类不可生成对象
             */
            virtual void format(std::ostream& os, std::shared_ptr<Logger> logger,
                                LogLevel::Level level, LogEvent::ptr event) = 0;
        };

        /**
         * @brief 初始化，解析日志模板
         */
        void init();

        /**
         * @brief 是否有错误
         */
        bool isError() const { return m_error ; }

        /**
         * @brief 返回日志模板
         */
        const std::string getPattern() const {  return m_pattern; }
    private:
        //日志格式模板
        std::string m_pattern;
        //日志格式解析后格式
        std::vector<FormatItem::ptr> m_items;
        //是否有错误
        bool m_error = false;
    };

    /**
     * @brief 日志输出目标
     */
    class LogAppender{
        friend class Logger;
    public:
        using ptr = std::shared_ptr<LogAppender>;
        using MutexType = Spinlock;

        /**
         * @brief 析构函数
         */
        virtual ~LogAppender() {}

        /**
         * @brief 写入日志
         * @param[in] logger 日志器
         * @param[in] level 日志级别
         * @param[in] event 日志事件
         * @note 纯虚函数，抽象基类不可生成对象
         */
        virtual void log(std::shared_ptr<Logger> logger, LogLevel::Level level,
                            LogEvent::ptr event) = 0;
        
        /**
         * @brief 将日志输出目标的配置转为YAML String
         */
        virtual std::string toYamlString() = 0;

        /**
         * @brief 更改日志格式器
         */
        void setFotmatter(LogFormatter::ptr val);
        
        /**
         * @brief 获取日志格式器
         */
        LogFormatter::ptr getFormatter();

        /**
         * @brief 获取日志级别
         */
        LogLevel::Level getLevel() const { return m_level; }

        /**
         * @brief 设置日志级别
         */
        void setLevel(LogLevel::Level value) { m_level = value; }
    protected:
        //日志级别
        LogLevel::Level m_level = LogLevel::DEBUG;
        //是否有自己的日志格式器
        bool m_hasFormatter = false;
        //Mutex
        MutexType m_mutex;
        //日志格式器
        LogFormatter::ptr m_formatter;
    };

    /**
     * @brief 日志器
     */
    class Logger
    :public std::enable_shared_from_this<Logger>            //公开继承
    {
        friend class LoggerManager;                         //友元类
    public:
        using ptr = std::shared_ptr<Logger>;
        using MutexType = Spinlock;

        /**
         * @brief 构造函数
         * @param[in] name 日志器名称
         */
        Logger(const std::string& name = "root");

        /**
         * @brief 写日志
         * @param[in] level 日志级别
         * @param[in] event 日志事件 
         */
        void log(LogLevel::Level level , LogEvent::ptr evnet);

        /**
         * @brief 写debug级别日志
         * @param[in] event 日志事件
         */
        void debug(LogEvent::ptr event);

        /**
         * @brief 写info级别日志
         * @param[in] event 日志事件
         */
        void info(LogEvent::ptr event);

        /**
         * @brief 写warn级别日志
         * @param[in] event 日志事件
         */
        void warn(LogEvent::ptr event);

        /**
         * @brief 写error级别日志
         * @param[in] event 日志事件
         */
        void error(LogEvent::ptr event);

        /**
         * @brief 写fatal级别日志
         * @param[in] event 日志事件
         */
        void fatal(LogEvent::ptr event);

        /**
         * @brief 添加日志目标
         * @param[in] appender 日志目标
         */
        void addAppender(LogAppender::ptr appender);

        /**
         * @brief 删除日志目标
         * @param[in] appender 日志目标
         */
        void delAppender(LogAppender::ptr appender);

        /**
         * @brief 清空日志目标
         */
        void clearAppenders();

        /**
         * @brief 返回日志级别
         */
        LogLevel::Level getLevel() const {  return m_level; }

        /**
         * @brief 设置日志级别
         */
        void setLevel(LogLevel::Level level) {  m_level = level; }

        /**
         * @brief 返回日志名称
         */
        const std::string& getName() const {    return m_name;  }

        /**
         * @brief 设置日志格式器
         */
        void setFormatter(LogFormatter::ptr val);

        /**
         * @brief 设置日志格式模板
         */
        void setFormatterPattern(const std::string& val);

        /**
         * @brief 获取日志格式器
         */
        LogFormatter::ptr getFormatter();

        /**
         * @brief 将日志器的配置转成YAML String
         */
        std::string toYamlString();
    private:
        //日志名称
        std::string m_name;
        //日志级别
        LogLevel::Level m_level;
        //Mutex
        MutexType m_mutex;
        //日志目标集合
        std::list<LogAppender::ptr> m_appenders;
        //日志格式器
        LogFormatter::ptr m_formatter;
        //主日志器
        Logger::ptr m_root;
    };

    /**
     * @brief 输出到标准输出的Appender
     */
    class StdoutLogAppender
    :public LogAppender
    {
    public:
        using ptr = std::shared_ptr<StdoutLogAppender>;

        /**
         * @brief 写入日志
         * @param[in] logger 日志器
         * @param[in] level 日志级别
         * @param[in] event 日志事件
         */
        void log(Logger::ptr logger, LogLevel::Level level, 
                                LogEvent::ptr event) override;
        
        std::string toYamlString() override;

    };

    /**
     * @brief 输出到文件的Appender
     */
    class FileLogAppender
    :public LogAppender
    {
    public:
        using ptr = std::shared_ptr<FileLogAppender>;

        /**
         * @brief 构造函数
         */
        FileLogAppender(const std::string& filename);

        /**
         * @brief 写入日志
         * @param[in] logger 日志器
         * @param[in] level 日志级别
         * @param[in] event 日志事件
         */
        void log(Logger::ptr logger, LogLevel::Level level, LogEvent::ptr event) override;

        /**
         * @brief 重载基类中的函数, 将日志器的配置转化为YAML string
         */ 
        std::string toYamlString() override;

        /**
         * @brief 重新打开日志文件
         * @return 成功打开文件
         */
        bool reopen();
    private:
        //文件路径
        std::string m_filename;
        //输出文件流
        std::ofstream m_outFileStream;
        //上次重新打开时间
        uint64_t m_lastTime = 0;
    };

    /**
     * @brief 日志器管理类
     */
    class LoggerManager{
    public:
        using MutexType = Spinlock;
        
        /**
         * @brief 构造函数
         */
        LoggerManager();

        /**
         * @brief 获取日志器
         * @param[in] name日志器名称
         */
        Logger::ptr getLogger(const std::string& name);

        /**
         * @brief 初始化
         */
        void init();

        /**
         * @brief 返回主日志器
         */
        Logger::ptr getRoot() const { return m_root; }

        /**
         * @brief 将所有日志器配置转成YAML String
         */
        std::string toYamlString();
    private:
        //Mutex
        MutexType m_mutext;
        //日志器容器
        std::map<std::string, Logger::ptr> m_loggers;
        //主日志器
        Logger::ptr m_root;
    };

    //TODO:实现将日志写入网络
    //TODO：实现环形日志文件

    //日志管理类单例模式
    using LoggerMgr = sylar::Singleton<LoggerManager> ;
}


#endif
