#include "Thread.h"
#include <stdio.h>

int add(int x,int y){
    return x+y;
}

int main(void){
    int x = 5,y = 6;
    Thread t(std::bind(add,x,y));
    x = 10;
    y = 21;
    Thread t2(std::bind(add,x,y));


    t.start();
    t2.start();
    t.join();
    t2.join();
}