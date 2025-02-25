//c library
#include <time.h>
// c++ library
#include <cstdio>
#include <cstring>
#include <map>
#include <iostream>
#include <functional>
// user define library
#include "log.h"
#include "util.h"
#include "config.h"
#include "macro.h"
#include "env.h"

namespace sylar{
const char* LogLevel::ToString(LogLevel::Level level) {
    switch(level) {
#define XX(name)    \
    case LogLevel::name: \
        return #name;/*#name是预处理器字符串化操作符，它会将宏参数转换为字符串字面量*/\
        break;
    
    XX(DEBUG);
    XX(INFO);
    XX(WARN);
    XX(ERROR);
    XX(FATAL);
#undef XX
    default:
        return "UNKNOWN";
    }
    return "UNKNOWN";
}

LogLevel::Level LogLevel::FromString(const std::string& str) {
#define XX(level, v) \
    if(str == #v) {\
        return LogLevel::level; \
    }

    //区分大小写，TODO:是否可以用lower函数将字符串先转换为小写后再用？要使用constexpr函数吗？？
    XX(DEBUG, debug);
    XX(INFO, info);
    XX(WARN, warn);
    XX(ERROR, error);
    XX(FATAL, fatal);

    XX(DEBUG, DEBUG);
    XX(INFO, INFO);
    XX(WARN, WARN);
    XX(ERROR, ERROR);
    XX(FATAL, FATAL);

    return LogLevel::UNKNOWN;
#undef XX
}

LogEventWrap::LogEventWrap(LogEvent::ptr e) 
:m_event(e)
{
    printf("LogEventWrap::LogEventWrap(LogEvent::ptr e) 构造函数\n");
}

LogEventWrap::~LogEventWrap() {
    m_event->getLogger()->log(m_event->getLevel(), m_event);
} 

void LogEvent::format(const char* fmt, ...) {
    va_list al;             //创建一个链表
    va_start(al, fmt);      //将初始化链表al,并传入参数列表的最后一个固定参数
    format(fmt, al);
    va_end(al);             //释放链表
}

void LogEvent::format(const char* fmt, va_list al) {
    char* buf = nullptr;
    int len = vasprintf(&buf, fmt, al);     //将格式化后的字符串存入在函数中动态分配的buf内存中，注意此处的二级指针
    if(len != -1) {
        m_ss << std::string(buf, len);      //构造string对象，并插入m_ss流对象
        free(buf);
    }
}

std::stringstream& LogEventWrap::getSS() {
    return m_event->getSS();
}

void LogAppender::setFotmatter(LogFormatter::ptr val) {
    MutexType::Lock lock(m_mutex);      //基于作用域的加锁
    m_formatter = val;
    if(m_formatter) {
        m_hasFormatter = true;
    } else {
        m_hasFormatter = false;
    }
}

LogFormatter::ptr LogAppender::getFormatter() {
    MutexType::Lock lock(m_mutex);
    return m_formatter;
}

class MessageFormatItem
:public LogFormatter::FormatItem
{
public:
    MessageFormatItem(const std::string& str = "") {}
    void format(std::ostream& os, Logger::ptr logger, LogLevel::Level level,
                LogEvent::ptr event) override {
        os << event->getContent();
    }
};

class LevelFormatItem
:public LogFormatter::FormatItem
{
public:
    LevelFormatItem(const std::string& str = "") {}
    void format(std::ostream& os, Logger::ptr logger, LogLevel::Level level,
                LogEvent::ptr event) override {
        os << LogLevel::ToString(level);
    }
};

class ElapseFormatItem
:public LogFormatter::FormatItem
{
public:
    ElapseFormatItem(const std::string& str = "") {}
    void format(std::ostream& os, Logger::ptr logger, LogLevel::Level level,
                LogEvent::ptr event) override {
        os << event->getElapse(); 
    }
};

class NameFormatItem
:public LogFormatter::FormatItem
{
public:
    NameFormatItem(const std::string & str = "") {}
    void format(std::ostream& os, Logger::ptr logger, LogLevel::Level level,
                LogEvent::ptr event) override {
        os << event->getLogger()->getName(); 
    }

};

class ThreadIdFormatItem
:public LogFormatter::FormatItem
{
public:
    ThreadIdFormatItem(const std::string & str = "") {}
    void format(std::ostream& os, Logger::ptr logger, LogLevel::Level level,
                LogEvent::ptr event) override {
        os << event->getThreadId();
    }
};

class FiberIdFormatItem
:public LogFormatter::FormatItem
{
public:
    FiberIdFormatItem(const std::string & str = "") {}
    void format(std::ostream& os, Logger::ptr logger, LogLevel::Level level,
                LogEvent::ptr event) override {
        os << event->getThreadName();
    }
};

class DateTimeFormatItem
:public LogFormatter::FormatItem
{
public:
    DateTimeFormatItem(const std::string& format = "%Y-%m-%d %H:%M:%S")
    :m_format(format)
    {
        if(m_format.empty()) {
            m_format = "%Y-%m-%d %H:%M:%S";
        }   
    }

    void format(std::ostream& os, Logger::ptr logger, LogLevel::Level level,
                LogEvent::ptr event) override {
        struct tm m;                                        //时间结构
        time_t time = event->getTimeStamp();                //获取系统时间
        localtime_r(&time, &m);    //_r代表是可递归的线程安全函数，将系统时间转换为本地时间
        char buf[64] = {'0'};
        strftime(buf, sizeof(buf), m_format.c_str(), &m);   //将时间格式化为字符串并存放在buf中，返回格式化后字符串的长度
        os << buf;                                          //将格式化后的字符串插入流中
    }
private:
    std::string m_format;
};

class FilenameFormatItem
:public LogFormatter::FormatItem
{
public:
    FilenameFormatItem(const std::string& str = "") {}
    void format(std::ostream& os, Logger::ptr logger, LogLevel::Level level,
                LogEvent::ptr event) override {
        os << event->getFile();
    }
};

class LineFormatItem
:public LogFormatter::FormatItem
{
public:
    LineFormatItem(const std::string& str = "") {}
    void format(std::ostream& os, Logger::ptr logger, LogLevel::Level level,
                LogEvent::ptr event) override {
        os << event->getLine();
    }
};

class NewLineFormatItem
:public LogFormatter::FormatItem
{
public:
    NewLineFormatItem(const std::string& str = "") {}
    void format(std::ostream& os, Logger::ptr logger, LogLevel::Level level,
                LogEvent::ptr event) override {
        os << std::endl; 
    }
};


class StringFormatItem
:public LogFormatter::FormatItem
{
public:
    StringFormatItem(const std::string& str = "") {}
    void format(std::ostream& os, Logger::ptr logger, LogLevel::Level level,
                LogEvent::ptr event) override {
        os << m_string; 
    }
private:
    std::string m_string;
};

class TabFormatItem
:public LogFormatter::FormatItem
{
public:
    TabFormatItem(const std::string& str = "") {}
    void format(std::ostream& os, Logger::ptr logger, LogLevel::Level level,
                LogEvent::ptr event) override {
        os << "\t"; 
    }
private:
    std::string m_string;
};

LogEvent::LogEvent(std::shared_ptr<Logger> logger,
                    LogLevel::Level level,
                    const char* file,
                    int32_t line,
                    uint32_t elapse,
                    uint32_t threadId,
                    uint32_t fiberId, 
                    uint64_t timeStamp,
                    const std::string& threadName )
    :m_file(file),      //类中私有变量初始化的顺序和声明顺序有关，而和此处的初始化顺序无关
    m_line(line),
    m_elapse(elapse),
    m_threadId(threadId),
    m_fiberId(fiberId),
    m_timeStamp(timeStamp),
    m_logger(logger),
    m_level(level)
    {

    }

Logger::Logger(const std::string& name)
:m_name(name),
m_level(LogLevel::DEBUG){
    m_formatter.reset(new LogFormatter("%d{%Y-%m-%d %H:%M:%S}%T%t%T%N%T%F[%p]%T[%c]%T%f:%l%T%m%n"));
}

void Logger::setFormatter(LogFormatter::ptr val) {
    MutexType::Lock lock(m_mutex);
    m_formatter = val;

    for(auto & i :m_appenders) {
        MutexType::Lock ll(i->m_mutex);
        if(!i->m_hasFormatter) {
            i->m_formatter = m_formatter;
        }
    };
    
}

void Logger::setFormatterPattern(const std::string& val) {
    std::cout << "---" << val << std::endl;
    sylar::LogFormatter::ptr new_val(new sylar::LogFormatter(val));
    if(new_val->isError()) {
        std::cout << "Logger setFormatter name = " << m_name
                  << " value = " << val << " invalid formatter"
                  << std::endl;
        return;
    }

    setFormatter(new_val);
}

std::string Logger::toYamlString() {
    MutexType::Lock lock(m_mutex);
    YAML::Node node;
    node["name"] = m_name;
    if(m_level != LogLevel::UNKNOWN) {
        node["level"] = LogLevel::ToString(m_level);
    }
    if(m_formatter) {
        node["formatter"] = m_formatter->getPattern();
    }

    for(auto& i :m_appenders) {
        node["appenders"].push_back(YAML::Load(i->toYamlString()));
    }

    std::stringstream ss;
    ss << node;
    return ss.str();
}

LogFormatter::ptr Logger::getFormatter() {
    MutexType::Lock lock(m_mutex);
    return m_formatter;
}

void Logger::addAppender(LogAppender::ptr appender) {
    MutexType::Lock lock(m_mutex);
    if(!appender->getFormatter()) {
        MutexType::Lock ll(appender->m_mutex);
        appender->m_formatter = m_formatter;
    }
    m_appenders.push_back(appender);
}

void Logger::delAppender(LogAppender::ptr appender) {
    MutexType::Lock lock(m_mutex);
    for(auto it = m_appenders.begin(); it != m_appenders.end(); ++it) {
        if(*it == appender) {
            m_appenders.erase(it);
            break;
        }
    }
}

//清空appender
void Logger::clearAppenders() {
    MutexType::Lock lock(m_mutex);
    m_appenders.clear();
}

void Logger::log(LogLevel::Level level, LogEvent::ptr event) {
    if(level >= m_level) {                  //过滤掉低于当前对象日志级别的日志
        auto self = shared_from_this();     //返回自身对象的std::shared_ptr对象
        MutexType::Lock lock(m_mutex);
        if(!m_appenders.empty()) {
            for(auto& i : m_appenders) {
                i->log(self, level, event);
            }
        }else if(m_root) {
            m_root->log(level, event);
        }
    }
}

void Logger::debug(LogEvent::ptr event) {
    log(LogLevel::DEBUG, event);
}
void Logger::info(LogEvent::ptr event) {
    log(LogLevel::INFO, event);
}

void Logger::warn(LogEvent::ptr event) {
    log(LogLevel::WARN, event);
}

void Logger::error(LogEvent::ptr event) {
    log(LogLevel::ERROR, event);
}

void Logger::fatal(LogEvent::ptr event) {
    log(LogLevel::FATAL, event);
}
FileLogAppender::FileLogAppender(const std::string& filename)
:m_filename(filename) {
    reopen();
}

void FileLogAppender::log(std::shared_ptr<Logger> logger, LogLevel::Level level,
                            LogEvent::ptr event) {
    if(level >= m_level) {
        uint64_t now = event->getTimeStamp();           //获取时间戳
        if(now >= (m_lastTime + 3)) {
            reopen();
            m_lastTime = now;
        }
        MutexType::Lock lock(m_mutex);
        if(!m_formatter->format(this->m_outFileStream, logger, level, event)) {
            std::cout << "error" << std::endl;
        }
    }
}

std::string FileLogAppender::toYamlString() {
    MutexType::Lock lock(m_mutex);
    YAML::Node node;
    //重载了node对象的[]运算符
    node["type"] = "FileLogAppender";
    node["file"] = this->m_filename;
    if(m_level != LogLevel::UNKNOWN) {
        node["level"] = LogLevel::ToString(m_level);
    }
    if(m_hasFormatter && m_formatter) {
        node["formatter"] = this->m_formatter->getPattern();
    }

    //字符串流
    std::stringstream ss;
    //node对象重载了输出运算符
    ss << node;     
    return ss.str();        
}

bool FileLogAppender::reopen() {
    MutexType::Lock lock(this->m_mutex);
    if(this->m_outFileStream) {
        this->m_outFileStream.close();
    }
    return FSUTIL::OpenForWrite(m_outFileStream, m_filename, std::ios::app);
}

void StdoutLogAppender::log(std::shared_ptr<Logger> loggerPtr, LogLevel::Level level, 
    LogEvent::ptr eventPtr) {
        if(level >= m_level) {
            MutexType::Lock lock(m_mutex);
            this->m_formatter->format(std::cout, loggerPtr, level, eventPtr);
        }
}
std::string StdoutLogAppender::toYamlString() {
    MutexType::Lock lock(m_mutex);
    YAML::Node node;
    node["type"] = "StdoutLogAppender";
    if(m_level != LogLevel::UNKNOWN) {
        node["level"] = LogLevel::ToString(m_level);
    }
    if(m_hasFormatter && m_formatter) {
        node["formatter"] = this->m_formatter->getPattern();
    }

    std::stringstream ss;
    ss << node;
    return ss.str();
}

LogFormatter::LogFormatter(const std::string& pattern) 
:m_pattern(pattern) {
    this->init();
}

std::string LogFormatter::format(std::shared_ptr<Logger> loggerPtr, LogLevel::Level level,
    LogEvent::ptr eventPtr) {
    std::stringstream ss;
    for(auto& i :this->m_items) {
            i->format(ss, loggerPtr, level, eventPtr);
    }
    return ss.str();
}

std::ostream& LogFormatter::format(std::ostream& ofs, std::shared_ptr<Logger> loggerPtr, 
            LogLevel::Level level, LogEvent::ptr eventPtr) {
        for(auto& i:this->m_items) {
            i->format(ofs, loggerPtr, level, eventPtr);
        }
        return ofs;
}

//%xxx %xxx{xxx} %%
void LogFormatter::init() {
    //str, format, type
    std::vector<std::tuple<std::string, std::string, int>> vec;
    std::string nstr;
    for(size_t i = 0; i < m_pattern.size(); ++i) {
        if(m_pattern[i] != '%') {
            nstr.append(1, m_pattern[i]);
            continue;
        }

        if((i + 1) < m_pattern.size()) {
            if(m_pattern[i + 1] == '%') {
                nstr.append(1, '%');
                continue;
            }
        }
        
        size_t n = i + 1;
        int fmt_status = 0;
        size_t fmt_begin = 0;

        std::string str;
        std::string fmt;
        while(n < m_pattern.size()) {
            if(!fmt_status && (!isalpha(m_pattern[n]) && 
                                m_pattern[n] != '{' && m_pattern[n != '}'])) {
                str = m_pattern.substr(i + 1, n- i - 1);
                break;
            }

            if(fmt_status == 0) {
                if(m_pattern[n] == '{') {
                    fmt = m_pattern.substr(fmt_begin + 1, n - fmt_begin - 1);
                    fmt_status = 0;
                    ++n;
                    break;
                }
            }
            ++n;
            if(n == m_pattern.size()) {
                if(str.empty()) {
                    str = m_pattern.substr(i + 1);
                }
            }
        }
        if(fmt_status == 0) {
            if(nstr.empty()) {
                vec.push_back(std::make_tuple(nstr, std::string(), 0));
                nstr.clear();
            }
            vec.push_back(std::make_tuple(str, fmt, 1));
            i = n - 1;
        }else if(fmt_status == 1) {
            std::cout << "pattern parse error: " << m_pattern << " - " << m_pattern.substr(i) << std::endl;
            m_error = true;
            vec.push_back(std::make_tuple("<<pattern_error", fmt, 0));      
        }
    }
    if(!nstr.empty()) {
        vec.push_back(std::make_tuple(nstr, "", 0));
    }
    static std::map<std::string, std::function<FormatItem::ptr(const std::string& str)>> s_format_items = {
#define XX(str, C) \
    {#str, [](const std::string& fmt) { return FormatItem::ptr(new C(fmt)); }}
        XX(m, MessageFormatItem),
        XX(p, LevelFormatItem),
        XX(r, ElapseFormatItem),
        XX(c, NameFormatItem),
        XX(t, ThreadIdFormatItem),
        XX(n, NewLineFormatItem),
        XX(d, DateTimeFormatItem),
        XX(f, FilenameFormatItem),
        XX(l, LineFormatItem),
        XX(T, TabFormatItem),
        XX(F, FiberIdFormatItem),
        XX(N, ThreadIdFormatItem)
#undef XX
    };

    for(auto& i :vec) {
        if(std::get<2>(i) == 0) {
            m_items.push_back(FormatItem::ptr(new StringFormatItem("<<error_format %" + std::get<0>(i) + ">>")));
            m_error = true;
        }else {
            auto it = s_format_items.find(std::get<0>(i));
            if(it == s_format_items.end()) {
                m_items.push_back(FormatItem::ptr(new StringFormatItem("<<error_format %" + std::get<0>(i) + ">>")));
                m_error = true;
            } else {
                m_items.push_back(it->second(std::get<1>(i)));
            }
        }
    }
}

/**
 * @brief 构造函数
 */
LoggerManager::LoggerManager() {
    //m_root不仅是个智能指针，而且也是个对象。所以能痛死使用直接成员运算符和间接成员运算符
    this->m_root.reset(new Logger);     //修改智能指针对象指向的原始值
    this->m_root->addAppender(LogAppender::ptr(new StdoutLogAppender));

    this->m_loggers[this->m_root->m_name] = this->m_root;
    init();
}

Logger::ptr LoggerManager::getLogger(const std::string& name) {
    MutexType::Lock lock(m_mutext);
    auto it = m_loggers.find(name);
    if(it != m_loggers.end()) {
        return it->second;
    }

    Logger::ptr logger(new Logger(name));
    logger->m_root = this->m_root;
    m_loggers[name] = logger;
    return logger;
}

struct LogAppenderDefine {
    int type = 0;   //  1 File, 2 Stdout
    LogLevel::Level level = LogLevel::UNKNOWN;
    std::string formatter;
    std::string file;
    bool operator==(const LogAppenderDefine& oth) const {
        return type == oth.type
                && level == oth.level
                && formatter == oth.formatter
                && file == oth.file;
    }

};

struct LogDefine {
    std::string name;
    LogLevel::Level level = LogLevel::UNKNOWN;
    std::string formatter;
    std::vector<LogAppenderDefine> appenders;

    bool operator==(const LogDefine& oth) const {
        return name == oth.name
                && level == oth.level
                && formatter == oth.formatter
                && appenders == oth.appenders;      //sylar中没有和oth.appenders比较
    }

    /**
     * @brief 自定义的比较运算符。用于std::set<LogDefine>
     */
    bool operator<(const LogDefine& oth) const {
        return name  < oth.name;
    }

    bool isValid() const {
        return !name.empty();
    }
};

//版本特化
template<>  
class LexicalCast<std::string, LogDefine> 
{
public:
    LogDefine operator()(const std::string& v) {
        YAML::Node n = YAML::Load(v);
        LogDefine ld;
        if(!n["name"].IsDefined()) {
            std::cout << "log config error: name is null, " << n <<std::endl;
            throw std::logic_error("log config name is null");
        }
        ld.name = n["name"].as<std::string>();
        ld.level = LogLevel::FromString(n["level"].IsDefined() ? n["level"].as<std::string>() : "");
        if(n["formatter"].IsDefined()) {
            ld.formatter = n["formatter"].as<std::string>();
        }

        if(n["appenders"].IsDefined()) {
            for(size_t x = 0; x < n["appenders"].size(); ++x) {
                auto a = n["appenders"][x];
                if(!a["type"].IsDefined()) {
                    std::cout << "log config error: appender type is null, " << a << std::endl;
                    continue;
                }
                std::string type = a["type"].as<std::string>();
                LogAppenderDefine lad;
                if(type == "FileLogAppender") {
                    lad.type = 1;
                    if(!a["file"].IsDefined()) {
                        std::cout << "log config error: fileappender file is null, " << a << std::endl;
                        continue;
                    }
                    lad.file = a["file"].as<std::string>();
                    if(a["formatter"].IsDefined()) {
                        lad.formatter = a["formatter"].as<std::string>();
                    }
                } else if(type == "StdoutLogAppender") {
                    lad.type = 2;
                    if(a["formatter"].IsDefined()) {
                        lad.formatter = a["formatter"].as<std::string>();
                    }
                }else {
                    std::cout << "log config error：appender type is invalid, " << a << std::endl;
                    continue;
                }

                ld.appenders.push_back(lad);
            }
        }
        return ld;
    }
};

template<>
class LexicalCast<LogDefine, std::string>
{
public:
    std::string operator()(const LogDefine& i) {
        YAML::Node n;
        n["name"] = i.name;
        if(i.level != LogLevel::UNKNOWN) {
            n["level"] = LogLevel::ToString(i.level);
        }

        if(!i.formatter.empty()) {
            n["formatter"] = i.formatter;
        }

        for(auto& a: i.appenders) {
            YAML::Node na;
            if(a.type == 1) {
                na["type"] = "FileLogAppender";
                na["file"] = a.file;
            }else if(a.type == 2) {
                na["type"] = "StdoutLogAppender";
            }

            if(a.level != LogLevel::UNKNOWN) {
                na["level"] = LogLevel::ToString(a.level);
            }

            if(!a.formatter.empty()) {
                na["formater"] = a.formatter;
            }

            n["appenders"].push_back(na);
        }
        //字符串流，内存IO
        std::stringstream ss;
        ss << n;                //YAML::Node对象重载了插入流运算符
        return ss.str();
    }
};

/**
 * @brief 全局变量
 */
sylar::ConfigVar<std::set<LogDefine>>::ptr g_log_defines = 
            sylar::Config::Lookup("logs", std::set<LogDefine>(), "logs config");

/**
 * @brief
 */
struct LogIniter
{
public:
    LogIniter() {
        g_log_defines->addListener([](const std::set<LogDefine>& old_value, 
                                        const std::set<LogDefine>& new_value){
            SYLAR_LOG_INFO(SYLAR_LOG_ROOT()) << "on_logger_conf_changed";
            for(auto& i:new_value) {
                auto it = old_value.find(i);    //it为迭代器
                sylar::Logger::ptr logger;
                if(it == old_value.end()) {
                    //新增logger
                    logger = SYLAR_LOG_NAME(i.name);
                }else {
                    if(!(i == *it)) {
                        //修改的logger
                        logger = SYLAR_LOG_NAME(i.name);
                    }else {
                        continue;
                    }
                }

                logger->setLevel(i.level);

                if(!i.formatter.empty()) {
                    logger->setFormatterPattern(i.formatter);
                }
                
                logger->clearAppenders();
                for(auto& a: i.appenders) {
                    sylar::LogAppender::ptr ap;
                    if(a.type == 1) {
                        ap.reset(new FileLogAppender(a.file));
                    }else if(a.type == 2) {
                        if(!sylar::EnvMgr::GetInstance()->has("d")) {
                            ap.reset(new StdoutLogAppender);
                        }else  {
                            continue;
                        }
                    }
                    ap->setLevel(a.level);
                    if(!a.formatter.empty()) {
                        LogFormatter::ptr fmt(new LogFormatter(a.formatter));
                        if(!fmt->isError()) {
                            ap->setFotmatter(fmt);
                        } else {
                            std::cout << "log.name = " << i.name << " appender type = " << a.type
                                        << " formatter = " << a.formatter << " is invalid" << std::endl; 
                        }
                    }
                    logger->addAppender(ap);
                }
            }
            for(auto& i :old_value) {
                auto it = new_value.find(i);
                if(it == new_value.end()) {
                    //删除logger
                    auto logger = SYLAR_LOG_NAME(i.name);
                    logger->setLevel((LogLevel::Level)0);
                    logger->clearAppenders();
                }
            }
        });
    }

};

//文件作用域的静态变量
static LogIniter __log_init;

std::string LoggerManager::toYamlString() {
    MutexType::Lock lock(m_mutext);
    YAML::Node node;
    for(auto i : m_loggers) {
        node.push_back(YAML::Load(i.second->toYamlString()));
    }

    std::stringstream ss;
    ss << node;
    return ss.str();
}

void LoggerManager::init() {

}

}
