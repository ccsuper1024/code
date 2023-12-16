/*  @file @brief @author chenchao @brief */
#ifndef  _TASKQUEUE_HPP
#define  _TASKQUEUE_HPP
#include "MutexLock.h"
#include "Condition.h"
#include <stddef.h>     //size_t
#include <queue>
#include <functional>
//using ElemType = Task*; //如果此语句不放在类外，则实现pop函数是会出现未标识错误
                          //原因是ElemType是类内的标识符，要加TaskQue::在ElemType前
class TaskQueue
{
    using ElemType = std::function<void()>;
public:
/* @file @brief @pram*/
    //禁止从整型隐式构造为一个TaskQueue
    TaskQueue(size_t queSize);
    ~TaskQueue();
    void push( ElemType&& value);    
    ElemType pop();
    //唤醒所有等待在非空条件下的所有线程
    void wakeup();

    //getsize
    int getsize() {
        return _que.size();
    }
    
    //被外部和内部调用
    bool empty();
    bool full();
private:
    size_t _queSize;
    std::queue<ElemType> _que;   //核心数据结构。任务队列中的元素被其他线程争用
    //因为引用类型必须在定义时就初始化。所以必须在类的构造函数的初始化列表中显示调用
    //此类型的构造函数，而不能调用默认构造函数。但是MutexLock只有一个默认无参构造函数，
    //所以必须不能使用引用类型。所以MutexLock要么使用非引用类型，要么使用指针类型
    MutexLock _mutex;       
    Condition _notEmpty;    //队列为空时线程要阻塞的条件变量
    Condition _notFull;     //队列为满时线程要阻塞的条件变量
    bool      _flag;        //让工作线程唤醒的时候，可以退出来
};
#endif      //_TASKQUEUE_HPP

