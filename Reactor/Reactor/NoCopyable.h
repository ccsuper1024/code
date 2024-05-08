//
// Created by chenchao on 24-5-8.
//

#ifndef REACTOR_NOCOPYABLE_H
#define REACTOR_NOCOPYABLE_H
class NoCopyable
{
public:
    //不可复制类必须自己定义默认构造和析构。或用=default指定编译器构造，因为复制构造会被删除，而有了
    //显式的删除的复制构造函数，编译器会认为不需要默认构造，不会自动合成默认构造函数
    NoCopyable()=default;
    ~NoCopyable()=default;
    NoCopyable(const NoCopyable& rhs)=delete;
    NoCopyable& operator=(const NoCopyable& rhs)=delete;
};
#endif //REACTOR_NOCOPYABLE_H
