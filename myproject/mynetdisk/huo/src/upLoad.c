#include "head.h"
#include "md5.h"
int upLoad(int sockfd, char *filename)
{
    int ret;

    //检查重名
    //int num;
    //recvn(sockfd, &num, sizeof(num));
    //if(num == 0)
    //{

        int fd = open(filename, O_RDONLY);
        send(sockfd,&fd,sizeof(fd),0);
        if(fd != -1)
        {


            //对上传文件计算MD5码
            MD5_CTX md5;
            MD5Init(&md5);

            char temp[8]={0}; 
            unsigned char decrypt[16]={0};  
            unsigned char decrypt32[33]={0};
            char hexbuf[40960];
            int fileLength = 0;
            while(1)
            {
                int read_len = read(fd, hexbuf,sizeof(hexbuf));
                if (read_len <0) {
                    printf("read ret = -1\n");
                    close(fd);
                    return -1;
                }
                if(read_len==0)
                {
                    break;
                }
                fileLength += read_len;
                MD5Update(&md5,(unsigned char *)hexbuf,read_len);
            }
            MD5Final(&md5,decrypt);
            strcat((char *)decrypt32,"");

            for(int i = 0; i < 16; i++)
            {
                sprintf(temp,"%02x",decrypt[i]);
                strcat((char *)decrypt32,temp);
            }

            //发送MD5
            send(sockfd, decrypt32, sizeof(decrypt32), 0);


            //接收已发送文件长度
            int  curSize;
            ret = recvn(sockfd, (void*)&curSize, sizeof(curSize));
            printf("curSize = %d\n", curSize);

            //发送文件总长度
            ret = send(sockfd, &fileLength, sizeof(fileLength), MSG_NOSIGNAL);
            ERROR_CHECK(ret, -1, "send");
            printf("filelength = %d\n", fileLength);


            //发送文件内容
            int sendSize = curSize;//已发送文件长度
            off_t off_t_curSize = (off_t)curSize;//设置起始发送位置
            while(sendSize < fileLength)
            {
                ret = sendfile(sockfd, fd, &off_t_curSize, fileLength - sendSize); 
                printf("ret = %d,off_t_cursize = %d,", ret,curSize);
                ERROR_CHECK(ret, -1, "sendfile"); 
                sendSize += ret;
                printf("sendSize = %d\n",sendSize);
            }

            printf("send success\n");
        }
        else
        {
            printf("open error!\n");

        }
   // }
   // else
   // {

    //    printf("same filename in same path!\n");
   // }
    return 0;
}
