/*  @file @brief @author chenchao @brief */
#ifndef  _TASKQUEUE_HPP
#define  _TASKQUEUE_HPP
#include "MutexLock.h"
#include "Condition.h"
#include <stddef.h>     //size_t
#include <queue>

class TaskQueue
{
public:
/* @file @brief @pram*/
    //禁止从整型隐式构造为一个TaskQueue
    TaskQueue(size_t queSize);
    ~TaskQueue();
    void push(const int& value);    //万能引用。可以接受，const值，左值，右值
    int pop();
    
    //被外部和内部调用
    bool empty();
    bool full();
private:
    size_t _queSize;
    std::queue<int> _que;   //核心数据结构。任务队列中的元素被其他线程争用
    //因为引用类型必须在定义时就初始化。所以必须在类的构造函数的初始化列表中显示调用
    //此类型的构造函数，而不能调用默认构造函数。但是MutexLock只有一个默认无参构造函数，
    //所以必须不能使用引用类型,要么使用非引用类型，要么使用指针类型
    MutexLock _mutex;       
    Condition _notEmpty;
    Condition _notFull;
};
#endif      //_TASKQUEUE_HPP

