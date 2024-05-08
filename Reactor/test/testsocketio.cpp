#include <unistd.h>
#include <iostream>
#include "../Reactor/SocketIO.h"
#include "../Reactor/Socket.h"
int main()
{
    Socket s;
    fprintf(stdout, "s.id() = %d",s.id());
    SocketIO sio(s.id());

    char str[] = "123"; //数组初始化的简写形式
    write(s.id(),str,4);

    char buf[4];
    sio.readn(buf,sizeof(buf));
    fprintf(stdout,"buf = %s",buf);
}
