/* @author chenchao @date @file @brief*/
# ifndef  _MAKE_UNIQUE_HPP
# define  _MAKE_UNIQUE_HPP
#include <memory>
#include <utility>
template<typename T, typename ...Args>   // 定义一个模板参数包
std::unique_ptr<T> make_unique(Args&& ...args) {    // 定义一个函数参数包
    return std::unique_ptr<T>(new T(std::forward<Args>(args)...));  // 解包
}

#endif

