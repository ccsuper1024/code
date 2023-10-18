#include <iostream>
#include "test.pb.h"

int main()
{
	SearchRequest request;
	request.set_query("xxx");
	std::cout << request.query() << std::endl;
	return 0;
}

