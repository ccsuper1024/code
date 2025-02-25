#ifndef SYLAR_THREAD_H
#define SYLAR_THREAD_H
#include "noncopyable.h"
#include "mutex.h"
namespace sylar{
    /**
     * @brief 线程类
     */
    class Thread
    :public Noncopyable
    {
        //线程智能指针类型
        using ptr = std::shared_ptr<Thread>;
    public:

        /**
         * @brief 构造函数
         * @param[in] cb 线程执行函数
         * @param[in] name 线程名称
         */
        Thread(std::function<void()> cb, const std::string *name);

        /**
         * @brief 获取当前的线程名称
         */
        static const std::string &GetName();

    private:
        std::string m_name;
    };

}
#endif // SYLAR_THREAD_H