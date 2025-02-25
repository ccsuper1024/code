/**
 * @file singleton.h
 * @brief 单例模式模板
 * @author chenchao
 * @date 20241009
 */

#ifndef SYLAR_SINGLETON_H
#define SYLAR_SINGLETON_H
#include <memory>

namespace sylar{
//匿名命名空间，具备全局作用域
namespace{
    template<class T, class X, int N>
    T& GetInstanceX() {
        static T v;
        return v;
    }

    template<class T, class X, int N>
    std::shared_ptr<T> GetInstancePtr() {
        static std::shared_ptr<T> v(new T);
        return v;
    }
}

/**
 * @brief 单例模式封装
 * @details T 类型
 *          X 为了创造多个实例对应的Tag
 *          N 同一个Tag创造多个实例索引
 */
template<class T, class X = void, int N = 0>
class Singleton {
public:
    /**
     * @brief 返回单例指针
     */
    static T* GetInstance() {
        static T v;
        return &v;
    }
};

/**
 * @brief 单例模式智能指针封装类
 * @details T 类型
 *          X 为了创造多个实例对应的Tag
 *          N 同一个Tag创造多个实例索引
 */
template<class T, class X = void, int N = 0 >
class SingletonPtr {
public:
    /**
     * @brief 返回单例智能指针
     */
    static std::shared_ptr<T> GetInstance() {
        static std::shared_ptr<T> v(new T);
        return v;
    }

};

}

#endif  //SYLAR_SINGLETON_H