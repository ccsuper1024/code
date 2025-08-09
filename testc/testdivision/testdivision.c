#include <stdio.h>
#include <stdint.h>

int main(void)
{
    int32_t value = 0;
    printf("1/value = %d\n", 1/value);          //未定义行为，不一定会报错  UB
    return 0;
}