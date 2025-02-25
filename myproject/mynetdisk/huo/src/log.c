#include"client.h"

void Info(char buf[])
{
 openlog("info",LOG_PID,LOG_LOCAL5);
 syslog(LOG_INFO, "file:%s,function: %s,line: %d,操作:%s ",__FILE__,__FUNCTION__,__LINE__,buf);
 closelog();
}
