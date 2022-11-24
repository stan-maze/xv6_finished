#include "kernel/types.h"
#include "user/user.h"
#include "kernel/fcntl.h"

void initlist(int end){
    int i;
    for(i=2; i<end; i++){
        // д���ַ��int���ȣ�����Ǳ������ڴ��еı�ʾ
        // �����˾ֲ�����ɾ��������
        write(1, &i, sizeof(int));
    }
}
// ���ڰ���ɸ
void Eratosthenes(int p){
    int n;
    // ������нṹ
    while (read(0, &n, sizeof(int)))
    {
        if(n % p != 0){
            // ͨ����һ����ɸ���������������ʸ������һ��ɸ
            // �����е�һ����������
            write(1, &n, sizeof(int));
        }
    }
}
void redirect(int k, int pd[]){
    // ����ÿ��ɸ����Ҫһ���µĶ��У�����׼��������ǹ�����
    // ������Ҫ����׼��������ض���ÿ�����̣����ӽ��̣����ԵĹܵ���
    close(k);
    dup(pd[k]);
    // ��֤�ܵ��ǹرյģ�����ѭ���ȴ�
    close(pd[0]);
    close(pd[1]);
}
void sink(){
    // һ��ɸ��һ������Ҳ����ȡ����������
    int pd[2];
    int start;
    if(read(0, &start, sizeof(int))){
        printf("prime %d\n", start);
        pipe(pd);
        if(fork()){
            // �Ѹ����̵��������ض���pd[0]
            // ��ʱ����pdȡ��������ʱ�ᱻ��������֤�����ݵİ��򵽴�
            redirect(0, pd);
            // ��һ��sink������ͼ��pd[0]������
            // ������ȥ����һ����
            sink();
        }else{
            // ��Ȼ�ӽ��̵��������ɸȥ�ɱ���λ������Ԫ����
            // �����ض���pd[1]����ϸ����̴�pd[0]��
            redirect(1, pd);
            // ����ɸ�㷨��ɸ�õ�����д��pd[1]
            Eratosthenes(start);
        }
    }
}
int main(int argc, char* argv[]){
    // main����һ�κ�sink������ȫһ���Ĺ���
    int end = 35;
    int pd[2];
    pipe(pd);
    if (fork()>0) {
    redirect(0, pd);
    sink();
    } else {
    redirect(1, pd);
    // Ψһ��ͬ��������ȡinitlist(p)
    initlist(end);
  }
  exit(0);
}