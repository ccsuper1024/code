#ifndef SYLAR_MUTEX_H
#define SYLAR_MUTEX_H

#include <pthread.h>
#include <semaphore.h>
#include <stdint.h>

#include <thread>
#include <functional>
#include <memory>
#include <atomic>
#include <list>

#include "fiber.h"
#include "noncopyable.h"
namespace sylar{
    /**
     * @brief 信号量
     */
    class Semaphore
    :public Noncopyable
    {
    public:
        /**
         * @brief 构造函数
         * @param[in] count 信号量值的大小
         */
        Semaphore(uint32_t count = 0);

        /**
         * @brief 析构函数
         */
        ~Semaphore();

        /**
         * @brief 获取信号量
         */
        void wait();

        /**
         * @brief 释放信号量
         */
        void notify();
    private:
        sem_t m_semaphore;
    };

    /**
     * @brief 局部锁的模板实现
     */
    template<class T>
    struct ScopedLockImpl
    {
    public:
        /**
         * @brief 构造函数
         * @param[in] mutex Mutex
         */
        ScopedLockImpl(T& mutex)
        :m_mutex(mutex){
            m_mutex.lock();
            m_locked = true;
        }

        /**
         * @brief 析构函数，自动释放锁
         */
        ~ScopedLockImpl(){
            unlock();
        }

        /**
         * @brief 加锁
         */
        void lock() {
            if(!m_locked) {
                m_mutex.lock();
                m_locked = true;
            }
        }
        
        /**
         * @brief 解锁
         */
        void unlock() {
            if(m_locked) {
                m_mutex.unlock();
                m_locked = false;
            }
        }
    private:
        //mutex
        T& m_mutex;

        //是否已经上锁
        bool m_locked;
    };

    /**
     * @brief 自旋锁
     */
    class Spinlock
    :public Noncopyable
    {
    public:
        //局部锁
        using Lock = ScopedLockImpl<Spinlock>;
        /**
         * @brief 局部锁
         */
        Spinlock() {
            pthread_spin_init(&m_mutex, 0);
        }

        /**
         * @brief 析构函数
         */
        ~Spinlock() {
            pthread_spin_destroy(&m_mutex);
        }

        /**
         * @brief 上锁
         */
        void lock() {
            pthread_spin_lock(&m_mutex);
        }

        /**
         * 解锁
         */
        void unlock() {
            pthread_spin_unlock(&m_mutex);
        }
    private:
        //自旋锁
        pthread_spinlock_t m_mutex;
    };
}


#endif  //end SYLAR_MUTEX_H