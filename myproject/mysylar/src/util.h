#ifndef SYLAR_UTIL_H
#define SYLAR_UTIL_H
#include <cxxabi.h>
#include <pthread.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/syscall.h>
#include <stdio.h>
#include <stdint.h>
#include <vector>
#include <string>
#include <iomanip>
#include <jsoncpp/json/json.h>
#include <iostream>
#include <yaml-cpp/yaml.h>
#include <boost/lexical_cast.hpp>
#include "util/json_util.h"
#include "util/hash_util.h"
#include "util/crypto_util.h"

namespace sylar{
    
/**
 * @brief 返回当前线程ID
 */
pid_t GetThreadId();

/**
 * @返回当前协程ID
 */
uint32_t GetFiberId();

void Backtrace(std::vector<std::string>& bt, int size = 64, int skip = 1);

std::string BacktraceToString(int size = 64, int skip = 2, const std::string& prefix = "");



class FSUTIL {
public:
    static bool OpenForRead(std::ifstream& ifs, const std::string& filename, 
                                std::ios_base::openmode mode);
    static bool OpenForWrite(std::ofstream& ifs, const std::string& filename, 
                                std::ios_base::openmode mode);
    

};

}
#endif //  SYLAR_UTIL_H   