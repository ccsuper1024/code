/**
 * @file noncopyable.h
 * @brief 不可拷贝类的封装
 * @author chenchao
 * @date 2024/10/06
 */
#ifndef SYLAR_NONCOPYABLE_H
#define SYLAR_NONCOPYABLE_H
namespace sylar
{
    /**
     * @brief 不可拷贝类
     */
    class Noncopyable
    {
    public:
        /**
         * 显式声明编译器合成的默认构造函数
         */
        Noncopyable() = default;
        /**
         * 显式声明编译器合成的默认析造函数
         */
        ~Noncopyable() = default;
        /**
         * 显式声明删除复制构造函数
         */
        Noncopyable(const Noncopyable &rhs) = delete;
        /**
         * 显式声明删除复制赋值运算符函数
         */
        Noncopyable &operator=(const Noncopyable &rhs) = delete;
    };

} // namespace sylar

#endif