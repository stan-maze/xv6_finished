
#include "kernel/types.h"
#include "user/user.h"
#include "kernel/fcntl.h"

int
main(int argc, char *argv[])
{
    if(argc!=2){
        write(1,"Usage: sleep <ticks>\n",strlen("Usage: sleep <ticks>\n"));
        exit(1);
    }
    int sleep_time = atoi(argv[1]);
    sleep(sleep_time);
    exit(0);
}