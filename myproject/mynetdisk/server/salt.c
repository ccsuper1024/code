#include"func.h"
#define _XOPEN_SOURCE  
#include <crypt.h>                                                                                                                       
#include <unistd.h>
#define STR_LEN 10 // 定义随机输出的字符串长度。
#include <mysql/mysql.h>
int GenerateStr(char* salt)
{
    
    int i, flag;
    srand(time(NULL)); // 通过时间函数设置随机数种子，使得每次运行结果随机。
    for (i = 0; i < STR_LEN; i++)
    {
        flag = rand() % 3;
        switch (flag)
        {
        case 0:
            salt[i] = rand() % 26 + 'a';
            break;
        case 1:
            salt[i] = rand() % 26 + 'A';
            break;
        case 2:
            salt[i] = rand() % 10 + '0';
            break;
        }
    }
    printf("%s\n", salt); // 输出生成的随机数。
    char zanshi[1024]="$6$";
      strcat(zanshi,salt);
      strcat(zanshi,"$");
      //printf("zanshi = %s\n",zanshi);
      memset(salt,0,sizeof(salt));
      for(size_t i=0;i<strlen(zanshi);i++){
        salt[i]=zanshi[i];
      }
      printf("salt = %s\n",salt);

    return 0;
}
