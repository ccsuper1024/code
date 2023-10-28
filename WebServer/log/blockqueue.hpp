/*
 * @file 
 * @brief 
 */
#ifndef BLOCKQUEUE_HPP
#define BLOCKQUEUE_HPP
#include <mutex>
#include <deque>    //容器适配器，默认底层容器是vector
#include <condition_variable>   //条件变量
#include <sys/time.h>
#include <cassert>

//模板类
template<class T>
class BlockQueue
{
public:
    explicit BlockQueue(size_t MaxCapacity = 1000);

    ~BlockQueue();

    //清空
    void clear();

    //判空
    bool empty();

    //判满
    bool full();

    //
    void close();

    //已存在元素容量大小
    size_t size();

    //可以存储元素的容量大小
    size_t capacity();

    //返回队头元素， 模板类的模板函数
    T front();

    //返回队尾元素
    T back();

    //尾插
    void push_back(const T& item);

    //头插
    void push_front(const T& item);

    //尾部弹出元素
    bool pop(T& item);

    //重载,函数的参数个数，顺序，类型不同
    bool pop(T& item, int timeout);

    //刷新
    void flush();
private:
    size_t _capacity;
    
    bool _isClose;

    std::deque<T> _deq;

    std::mutex _mtx;

    std::condition_variable _condConsumer;

    std::condition_variable _condProducer;
};

template<class T>
BlockQueue<T>::BlockQueue(size_t MaxCapacity)
:_capacity(MaxCapacity)
{
    assert(_capacity > 0);
    _isClose = false;
}

template<class T>
BlockQueue<T>::~BlockQueue() 
{
    this->close();
}

//清空
template<class T>
void BlockQueue<T>::clear() 
{
    
}

//判空
template<class T>
bool BlockQueue<T>::empty() 
{

}

//判满
template<class T>
bool BlockQueue<T>::full() 
{

}

//
template<class T>
void BlockQueue<T>::close() 
{
    {
        std::lock_guard<std::mutex> lock(_mtx); //自动加解锁，基于作用域的范围锁
        _deq.clear();
        _isClose = true;
    }
    //通知所有在此条件变量下等待的线程
    _condProducer.notify_all();
    _condConsumer.notify_all();
}

//已存在元素容量大小
template<class T>
size_t BlockQueue<T>::size() 
{
    std::lock_guard<std::mutex> lock(_mtx);
    return _deq.size();
}

//可以存储元素的容量大小
template<class T>
size_t BlockQueue<T>::capacity() 
{
    std::lock_guard<std::mutex> lock(_mtx);
    return _capacity;
}

//返回队头元素， 模板类的函数
template<class T>
T BlockQueue<T>::front() 
{
    std::lock_guard<std::mutex> lock(_mtx);
    return _deq.front();
}

//返回队尾元素
template<class T>
T BlockQueue<T>::back() 
{
    std::lock_guard<std::mutex> lock(_mtx);
    return _deq.back();
}

//尾插
template<class T>
void BlockQueue<T>::push_back(const T& item) 
{
    //独占式的锁，只能移动不可复制。std::shared_lock<std::mutex>类是共享式的锁，用于实现读写锁
    std::unique_lock<std::mutex> lock(_mtx);
    while(_deq.size() >= _capacity){
        _condProducer.wait(lock);   //生产者在条件变量阻塞等待
    }
    _deq.push_back(item);
    _condConsumer.notify_one();//至少唤醒一个线程，避免虚假唤醒造成的线程争抢开销
}

//头插
template<class T>
void BlockQueue<T>::push_front(const T& item) 
{
    std::unique_lock<std::mutex> lock(_mtx);
    while(_deq.size() >= _capacity){
       _condProducer.wait(lock); 
    }
    _deq.push_front(item);
    _condConsumer.notify_one();
}

//尾部弹出元素
template<class T>
bool BlockQueue<T>::pop(T& item) 
{
    std::unique_lock<std::mutex> lock(_mtx);
    while(_deq.empty()){
        _condConsumer.wait(lock);   //当队列为空则消费者线程等待
        if(_isClose){
            return false;
        }
    }
    //若队列不为空则从队列中取出元素，通知生产者线程
    item = _deq.front();
    _deq.pop_front();
    _condProducer.notify_one();

    return true;
}

//重载,函数的参数个数，顺序，类型不同
template<class T>
bool BlockQueue<T>::pop(T& item, int timeout) 
{
    std::unique_lock<std::mutex> lock(_mtx);
    while(_deq.empty()){
        if(_condConsumer.wait_for(lock,std::chrono::seconds(timeout))
           == std::cv_status::timeout){
            return false;
        }
        if(_isClose){
            return false;
        }
    }
    item = _deq.front();
    _deq.pop_front();
    _condProducer.notify_one();

    return true;
}

//通知消费者线程来消费
template<class T>
void BlockQueue<T>::flush() 
{
    _condConsumer.notify_one();
}

#endif  //BLOCKQUEUE_HPP
