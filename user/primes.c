#include "kernel/types.h"
#include "user/user.h"
#include "kernel/fcntl.h"

void initlist(int end){
    int i;
    for(i=2; i<end; i++){
        // 写入地址后int长度，这就是变量在内存中的表示
        // 避免了局部变量删除的问题
        write(1, &i, sizeof(int));
    }
}
// 基于埃氏筛
void Eratosthenes(int p){
    int n;
    // 很像队列结构
    while (read(0, &n, sizeof(int)))
    {
        if(n % p != 0){
            // 通过第一个数筛，不被整除的有资格进入下一轮筛
            // 而其中第一个必是素数
            write(1, &n, sizeof(int));
        }
    }
}
void redirect(int k, int pd[]){
    // 由于每次筛都需要一个新的队列，而标准输入输出是公共的
    // 所以需要将标准输入输出重定向到每个进程（和子进程）独自的管道中
    close(k);
    dup(pd[k]);
    // 保证管道是关闭的，避免循环等待
    close(pd[0]);
    close(pd[1]);
}
void sink(){
    // 一次筛出一个数，也就是取出数列首项
    int pd[2];
    int start;
    if(read(0, &start, sizeof(int))){
        printf("prime %d\n", start);
        pipe(pd);
        if(fork()){
            // 把父进程的输入流重定向到pd[0]
            // 此时当从pd取不到数据时会被阻塞，保证了数据的按序到达
            redirect(0, pd);
            // 下一次sink将会试图从pd[0]拿数据
            // 父进程去拿下一个数
            sink();
        }else{
            // 显然子进程的任务就是筛去可被首位整除的元素了
            // 首先重定向到pd[1]，配合父进程从pd[0]拿
            redirect(1, pd);
            // 埃氏筛算法把筛好的数据写入pd[1]
            Eratosthenes(start);
        }
    }
}
int main(int argc, char* argv[]){
    // main先跑一次和sink几乎完全一样的过程
    int end = 35;
    int pd[2];
    pipe(pd);
    if (fork()>0) {
    redirect(0, pd);
    sink();
    } else {
    redirect(1, pd);
    // 唯一不同的是这里取initlist(p)
    initlist(end);
  }
  exit(0);
}