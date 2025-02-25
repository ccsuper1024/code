#include <iostream>

class A
{
public:
    A(){    }
    A(int a):_a(a)  {}
    A(const A& a):_a(a._a) {}
    private:
    int _a;
};
int c = 1;
A d;
class B
{
public:
    B():a(d)
    {

    }
private:
    A& a;
};