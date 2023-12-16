/*  @file @brief @author chenchao @brief */
#ifndef  _NOCOPYABLE_HPP
#define  _NOCOPYABLE_HPP
//删除复制拷贝函数和赋值运算符函数
class NoCopyable
{
public:
/* @file @brief @pram*/
    //有了自定义的构造函数后，编译器不会再合成构造函数了,所以为了派生类中可以调用
    //此基类的默认无参构造函数，应该自定义，或者用=default来定义默认无参构造函数
    NoCopyable()=default;
    NoCopyable(const NoCopyable& )=delete;
    NoCopyable& operator=(const NoCopyable& )=delete;
    ~NoCopyable()=default;

private:

};
#endif      //_NOCOPYABLE_HPP

