/*  @file @brief @author chenchao @brief */
#include <boost/python.hpp>

char const* greet() {
    return "hello world";
}

BOOST_PYTHON_MODULE(hello_exxt) {
    using namespace boost::python;
    def("greet",greet);
}

/* @brief @pram @return */
