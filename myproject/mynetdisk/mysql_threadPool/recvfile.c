#include "threadPool.h"

int recvn_t(int netfd, void *buf, int length)
{
    char *pbuf = (char *)buf;
    int total = 0;
    while(total < length)
    {
        int ret = recv(netfd, pbuf + total, length - total, 0);
        total += ret;
        printf("total = %d\n", total);
    }
    return 0;
}

int recvFile(int netfd, char *filename)
{
    int fd = open(filename, O_RDWR | O_CREAT | O_TRUNC, 0666);
    ERROR_CHECK(fd, -1, "open");

    int fileLength; 
    recvn_t(netfd, &fileLength, sizeof(fileLength));
    
    ftruncate(fd, fileLength);
    printf("filelength = %d\n", fileLength);
    char *pFile = (char *)mmap(NULL, fileLength, PROT_WRITE | PROT_READ,MAP_SHARED, fd, 0); 
    ERROR_CHECK(pFile, MAP_FAILED, "mmap");

    recvn_t(netfd, pFile, fileLength);
    printf("recvFile success\n");
    return 0;
}
