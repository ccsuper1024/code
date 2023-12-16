/* @author chenchao @date @file 
    @brief Pimpl(Pointer to implementation)是一种减少代码依赖和编译时间的c++编程技巧
            其基本思想是将一个外部可见类的实现细节(一般是私有的非虚成员)放在一个单独的
            实现类中，而在可见类中通过一个私有指针来间接访问该实现类
            即将经常变动的部分分出去作为一个单独的类实现，这样，每次编译时不会总是从
            头开始，不管类的那部分变动还是没变动都要重新编译一遍。白费时间
编译时Person::Impl是一个不完整的类型，所以如果编译器在头文件中自动合成的特殊函数需要进行
类型完整性检查则会导致编译失败！下面是编译器自动生成的特殊函数的行为：
    析构函数: std::unique_ptr<Person::Impl>使用默认的deleter，调用delete之前，
        会用static_assert 在编译阶段对Person::Impl进行类型完整性检查，确保内部裸指针
        不会指向一个不完整的类型。
    移动赋值操作符：在赋值之前，需要销毁赋值操作符左边的Person对象，而销毁时需要对
        Person::Impl进行类型完整性检查。
    移动构造函数：编译器通常会在出现异常时生成销毁Person对象，而销毁时需要对
        Person::Impl进行类型完整性检查。
    拷贝构造函数和拷贝赋值操作符：默认产生的是浅拷贝, 只拷贝了std::unique_ptr；而我们
        可能需要深拷贝，拷贝指针指向的内容。
编译器自动生成的析构函数、移动赋值操作符和移动构造函数需要对Person::Impl进行
类型完整性检查，所以应该放置在源文件中；而编译器自动生成的拷贝构造函数和拷贝赋值操作符
采用的是浅拷贝，如果要实现深拷贝也应该在源文件中定义
    */
#include <memory>
#include <iostream>

using std::shared_ptr;
using std::unique_ptr;
using std::endl;
using std::cout;
using std::cerr;
using std::cin;

struct Impl;    //前向声明。c++遵循一次定义原则，即只能定义一次，但可以声明多次
class Person
{
public:
    Person(const std::string& name);
    void print();
    ~Person()=default;
private:
    std::unique_ptr<Impl> _pimpl;
};
struct Impl
{
    std::string _name;
    Impl(const std::string& name)
    :_name(name)
    {}

    void print() {
        cout << "name: " << _name << endl;
    }
};
//这两个函数必须在struct Impl之后定义，如果在class Person中定义，因为struct Impl是
//一个不完整类型，所以编译器无法调用Impl的构造函数和print()成员函数。究其原因还是因为
//c++的编译模型是单遍编译的，即从头到尾只走一次(现代编译器中有些地方不是，但大多是时是的)
//所以要实现不完整类型中成员函数的调用就要把函数的实现放到不完整类型实现之后实现
Person::Person(const std::string& name )
:_pimpl(new Impl(name))
{}

void Person::print() {
    _pimpl->print();
}
/* @brief @pram @return */
void test(){
    Person person(std::string("aaa"));
    person.print();
}
int main()
{
    test();
    return 0;
}

