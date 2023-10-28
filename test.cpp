/**
 * @file 
 * @brief 
 */
#include <iostream>



using std::cout;
using std::cin;
using std::endl;
using std::cerr;


/**
 * @brief 
 * @param 
 * @param 
 * @return 
 */
int main()
{
    int a = 10;
    int* p = &a;
    cout << "a = " << a << endl;
    cout << "p = " << p << endl;
    ++*p;
    cout << "after:" << endl;
    cout << "a = " << a << endl;
    cout << "p = " << p << endl;

    return 0;
}

