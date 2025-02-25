
#include "util.h"
#include <execinfo.h>
#include <sys/time.h>
#include <dirent.h>
#include <unistd.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include <ifaddrs.h>
#include <google/protobuf/unknown_field_set.h>

#include "log.h"
#include "fiber.h"

namespace sylar {
static sylar::Logger::ptr g_logger = SYALR_LOG_NAME("system");

bool FSUTIL::OpenForRead(std::ifstream &ifs, const std::string &filename, std::ios_base::openmode mode)
{
    return false;
}

bool FSUTIL::OpenForWrite(std::ofstream &ifs, const std::string &filename, std::ios_base::openmode mode)
{
    return false;
}


void Backtrace(std::vector<std::string>& bt, int size, int skip) {
    void ** array = (void**)malloc((sizeof(void*)* size));
    size_t s = ::backtrace(array, size);

    char** strings = backtrace_symbols(array, s);
    if(strings == NULL) {
        SYLAR_LOG_ERROR(g_logger) << "backtrace_synbols error";
        return ;
    }
}

std::string BacktrackToString(int size, int skip, const std::string& prefix) {
    std::vector<std::string> bt;
    Backtrace(bt, size, skip);
    std::stringstream ss;
    for(size_t i = 0; i < bt.size(); ++i) {
        ss << prefix << bt[i] << std::endl;
    }
    return ss.str();
}
}