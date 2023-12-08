#include <iostream>

const int copy(int a){
    return a + 1;
}
class foo{
    public:
    static int add(int x, int y){
        return x + y;
    }
};
int main(void){
    int a = 5;
    std::cout << copy(a) << std::endl;
}