#ifndef SYLAR_MACRO_H
#define SYLAR_MACRO_H
#include <string.h>
#include <assert.h>
#include "log.h"
#include "util.h"

#if defined  __GUNC__ || defined __llvm__
//LIKELY 宏的封装，告诉编译器优化，条件大概率成立
#define SYLAR_LIKELY(x)     __builtin_expect(!!(x), 1)
//LIKELY 宏的封装， 告诉编译器优化，条件大概率不成立
#define SYLAR_UNLIKELY(x)   __builtin_expect(!!(x), 0)
#else
#define SYLAR_LIKELY(x)     (x)
#defien SYLAR_UNLIKELY(x)   (x)
#endif

//断言宏封装
#define SYLAR_ASSERT(x) \
    if(SYLAR_UNLIKELY(!(x))) { \
        SYLAR_LOG_ERROR(SYLAR_LOG_ROOT()) << "ASSERTION: " #x \
                        << "\nbacktrace:\n" \
                        << sylar::BacktraceToString(100,2,"     ");\
        assert(x);\
    }
#define SYLAR_ASSERT2(x, w) \
    if(SYLAR_UNLIKELY(!(x))) { \
        SYLAR_LOG_ERROR(SYLAR_LOG_ROOT()) << "ASSERTION: " #x \
                        << "\n" << w \
                        << "\nbacktrace:\n" \
                        << sylar::BacktraceToString(100,2,"     ");\
        assert(x);\
    }
#endif  //SYLAR_MACRO_H