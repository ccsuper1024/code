#include <func.h>

typedef  struct path_s{
     char* root;//定死的用户根目录
     char* path;//客户端的目录
 }path_t;
int list(path_t rootuser ,const char* pathpara, int netfd);
int removeThing(path_t rootuser ,const char* pathpara,int netfd);
