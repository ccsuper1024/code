#pragma once
#include <sys/stat.h>
#include <sys/sendfile.h>
#include <func.h>
#include <sys/mman.h>
int upLoad(int sockfd, char *filename);
int recvFile(int sockfd, char *filename);
