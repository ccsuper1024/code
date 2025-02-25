/**
 * @file fiber.h
 * @brief 协程封装
 * @author chenchao
 * @date 2024-10-17
 */

#ifndef SYLAR_FIBER_H
#define SYLAR_FIBER_H
//C Library
#include <ucontext.h>
//C++ Library
#include <memory>
#include <functional>

namespace sylar
{
//类的前向声明
class Scheduler;

/**
 * @brief 协程类
 */
class Fiber
:public std::enable_shared_from_this<Fiber>
{
    friend class Scheduler;
public:
    using ptr = std::shared_ptr<Fiber>;

    /**
     * @brief 协程状态，状态机实现协程
     */
    enum State { //非限定作用的enum声明，State枚举类型拥有Fiber类的作用域
        //初始状态
        INIT,
        //暂停状态
        HOLD,
        //执行中状态
        EXEC,
        //结束状态
        TERM,
        //可执行状态
        READY,
        //异常状态
        EXCEPT
    };
private:
    /**
     * @brief 无参构造函数
     * @attention 每个线程第一个协程的构造
     */
    Fiber();

public:
    /**
     * @brief 有参构造函数
     * @param[in] cb 协程执行的函数
     * @param[in] stacksize 协程栈大小
     * @param[in] use_caller 是否在MainFiber上调度
     */
    Fiber(std::function<void()> cb, size_t stacksize = 0, bool use_caller = false);

    /**
     * @brief 析构函数
     */
    ~Fiber();

    /**
     * @brief 重置协程执行函数，并设置状态
     * @pre getState() 为INIT, TERM, EXCEPT
     * @post getState() = INIT
     */
    void reset(std::function<void()> cb);

    /**
     * @brief 将当前协程切换到运行状态
     * @pre getState != EXEC
     * @post getState() = EXEC
     */
    void swapIn();

    /**
     * @brief 将当前协程切换到后台
     */
    void swapOut();

    /**
     * @brief 将当前线程切换到执行状态
     * @pre 执行的为当前线程的主协程
     */
    void call();

    /**
     * @brief 将当前线程切换到后台
     * @pre 执行的为该写成
     * @post 返回到线程的主写成
     */
    void back();

    /**
     * @brief 返回协程id
     */
    uint64_t getId() const {    return m_id;    }

    /**
     * @brief 返回协程状态
     */
    State getState() const {    return m_state; }
public:
    /**
     * @brief 设置当前线程的运行协程， 静态函数
     * @param[in] f 运行协程
     */
    static void SetThis(Fiber* f);

    /**
     * @brief 返回当前所在的写成
     */
    static Fiber::ptr GetThis();

    /**
     * @brief 将当前协程切换到后台，并设置为READY状态
     * @post getState()=READY
     */
    static void YieldToHold();

    /**
     * @brief 返回当前协程的总数量
     */
    static uint64_t TotalFibers();

    /**
     * @brief 协程执行函数
     * @post 执行完成返回到线程主协程
     */
    static void MainFunc();

    /**
     * @brief 协程执行函数
     * @post 执行完成返回到线程调度协程
     */
    static void CallerMainFunc();

    /**
     * @brief 获取当前协程的id
     */
    static uint64_t GetFiberId();
private:
    //协程id
    uint64_t m_id = 0;
    //协程运行栈大小
    uint32_t m_stacksize = 0;
    //协程状态
    State m_state = INIT;
    //协程上下文
    ucontext_t m_ctx;
    //协程运行栈指针
    void* m_stack = nullptr;
    //协程运行函数
    std::function<void()> m_cb;

};

}

#endif  // SYLAR_FIBER_H     