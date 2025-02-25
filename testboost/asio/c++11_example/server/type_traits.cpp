/*
#include<type_traits>中包含了很多的模板类用于在编译时进行类型信息的检查和转换
可以在编译时知道静态类型的属性，如：是否是指针类型，是否是引用类型，是否是基本类型
要小心动态类型，即在运行时才能决定具体类型的类型。如指向派生类对象的基类指针，即多态
*/

#include<type_traits>
#include<iostream>

//模板类
template<typename T>
void CheckPointerType() {
    if(std::is_pointer<T>::value) {
        std::cout << "Type is a pointer " << std::endl;
    }else {
        std::cout << "Type is not a ponter " << std::endl;
    }
}

template<typename T> 
void CheckReferenceType() {
    if(std::is_reference<T>::value) {
        std::cout << "Type is a reference" << std::endl;
    } else {
        std::cout << "Type is not a reference" << std::endl;
    }
}

template<typename T>
void PrintTypeInfo(){
    //由于模板依赖的原因，编译器可能无法确定模板类或模板函数中传递的T是否是一个类型
    //所以要在模板参数中加上typename来作为强调
    std::cout << "Original type: " << typeid(T).name() << std::endl;
    std::cout << "Pointer type: " << typeid(typename std::remove_pointer<T>::type).name() << std::endl;
    std::cout << "Reference type: " << typeid(typename std::remove_reference<T>::type).name() << std::endl;
    std::cout << "Naked type: " << typeid(typename std::remove_cv<T>::type).name() << std::endl; //此处的cv指的是 const valatile
}
int main(void) {
    CheckPointerType<int>();
    CheckPointerType<int*>();

    int x = 10;
    CheckReferenceType<int>();
    CheckReferenceType<int&>();
    CheckReferenceType<decltype(x)>();  //输出：Type is not a reference

    PrintTypeInfo<int*>();  //输出： int,int, int, int
    PrintTypeInfo<int&>();  //输出：int,int,int,int
    PrintTypeInfo<const int>(); //输出const int, const int ,int, int

    return 0;
}