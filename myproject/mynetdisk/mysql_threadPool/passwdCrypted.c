#include "threadPool.h"
int passwdCrypted(const char *name,const char *passwd){
    struct spwd *sp;
    sp = getspnam(name);
    char salt[256] = {0};
    int i,j;
    for(i = 0,j =0;sp->sp_pwdp[i] && j != 3;++i){
        if(sp->sp_pwdp[i] == '$'){
            ++j;
        }
    }
    strncpy(salt,sp->sp_pwdp,i-1);
    if(strcmp(sp->sp_pwdp,crypt(passwd,salt)) != 0){
        return -1;
    }else{
        return 0;
    }
}
