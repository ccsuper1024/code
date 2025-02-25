#include "head.h"

int upLoad(int netfd, char *filename)
{
    int fd = open(filename, O_RDONLY);
    ERROR_CHECK(fd, -1, "open");
    
    struct stat statbuf;
    int ret = fstat (fd, &statbuf);
    ERROR_CHECK(ret, -1, "fstat");
    int fileLength = statbuf.st_size;

    ret = send(netfd, &fileLength, sizeof(fileLength), 0);
    ERROR_CHECK(ret, -1, "send");

    int curSize = 0;
    while(curSize < fileLength)
    {
        ret = sendfile(netfd, fd, NULL, fileLength - curSize); 
        ERROR_CHECK(ret, -1, "sendfile"); 
        curSize += ret;
    }

    printf("send success");
    
}
