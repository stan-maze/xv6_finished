
#include "kernel/types.h"
#include "user/user.h"
#include "kernel/fcntl.h"
int
main(int argc, char *argv[])
{
    // father_to_child 和 child_to_father管道
    int p_ftoc[2],p_ctof[2];
    pipe(p_ftoc);
    pipe(p_ctof);
    // 写入father_to_child[1]
    write(p_ftoc[1],"received ping\n",strlen("received ping pong\n"));
    // 注意关闭，否则可能出现循环等待
    close(p_ftoc[1]);
    if(!fork()){
        char buf[512];
        // 从写入father_to_child[0]中取出received ping pong
        int n = read(p_ftoc[0],buf,sizeof(buf));
        close(p_ftoc[0]);
        if(n<=0){ 
            write(1,"children faild\n",strlen("children faild\n"));
            exit(-1);
        }
        printf("%d: %s",getpid(),buf);
        // 写入child_to_father[1]
        write(p_ctof[1],"received pong\n",strlen("received pong\n"));
        close(p_ctof[1]);
        exit(0);
    }
    else{
        char res[512];
        // 从child_to_father[0]取出received ping pong
        int n = read(p_ctof[0],res,sizeof(res));
        close(p_ctof[0]);
        if(n<=0) {
            write(1,"father faild\n",strlen("father faild\n"));
            exit(-1);
        }
        printf("%d: %s",getpid(),res);
        exit(0);
        }
}
