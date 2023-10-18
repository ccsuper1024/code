// test.cpp
#include <iostream>
#include <boost/array.hpp>
#include <boost/version.hpp>

using namespace std;
int main(){
   boost::array<int, 4> arr = {{1,2,3,4}};
   cout << "hi" << arr[0] << endl;
   cout << "BOOST VERSION: " << BOOST_LIB_VERSION << endl;
   return 0;
}

