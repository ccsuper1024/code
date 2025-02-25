#include <func.h>
int Passwd(const char *name,const char *passwd){
    struct spwd *sp;
    uid_t uid = getuid();
    if(setuid(0) == 0){
        sp = getspnam(name);
    }else{
        printf("error\n");
    }
    setuid(uid);
    char salt[256] = {0};
    int i,j;
    for(i = 0,j = 0;sp->sp_pwdp[i] && j!= 3;++i){
        if(sp->sp_pwdp[i] == '$'){
            ++j;
        }
    }
    strncpy(salt,sp->sp_pwdp,i-1);
    if(strcmp(sp->sp_pwdp,crypt(passwd,salt)) == 0){
        printf("true\n");
    }else{
        printf("flase\n");
    }
    return 0;
}
int main(){
    char name[] = "test2";
    char passwd [] = "123";
    Passwd(name,passwd);
    return 0;
}
