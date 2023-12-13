/* @author chenchao @date @file @brief*/
#ifndef  _NOCOPYABLE_HPP
#define  _NOCOPYABLE_HPP

//禁止复制。当被继承后，派生类也无法复制，因为派生类对象的基类部分无法复制
class Nocopyable
{
public:
/* @file @brief @pram*/
    //必须自定义或用=default修饰构造函数，因为有了自定的复制构造函数后，不会合成构造函数
    Nocopyable()=default;
    //复制构造函数禁止
    Nocopyable(const Nocopyable& rhs)=delete;
    Nocopyable& operator=(const Nocopyable&rhs)=delete;
    ~Nocopyable()=default;
private:

};
#endif  //_NOCOPYABLE_HPP

