#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
#include "kernel/param.h"

#define MAXLEN 1024

int main(int argc, char *argv[]) {
    char *_argv[MAXARG];  // exec������Ҫִ�еĲ���
    char buf[MAXLEN];  // �ӱ�׼���봫��Ĳ�������\0����
    char ch; //ÿ�ζ�ȡ��һ�ֽ�
    int stat;

    if (argc < 2) {
        fprintf(2, "usage: xargs command\n");
        exit(1);
    }

    for (int i = 1; i < argc; i++){  // ��args��Ĳ�����������б�
        _argv[i - 1] = argv[i];
    }

    while (1) { // ��׼�����л�������
        int index = 0;    // bufβָ�룬��ʾ��ǰbuf����䵽��λ��
        int arg_begin = 0;  // ��ʾ��ǰ������buf�п�ʼ��λ��
        int argv_cnt = argc - 1;
        while (1) { // ��ȡһ�У����س�����
            stat = read(0, &ch, 1);  //�ӱ�׼�����ж�ȡ
            if (stat == 0)          // û���������˳�
                exit(0);
            if (ch == ' ' || ch == '\n') {
                buf[index++] = '\0';  //�����ַ���֮����\0����
                _argv[argv_cnt++] = &buf[arg_begin]; //����ǰ�����׵�ַ���ݸ���������
                arg_begin = index; //�ƶ�����һ��������ͷ��
                if (ch == '\n') break; //��������˻س���������������
            } else {
                buf[index++] = ch; //����ȡ���ֽ�д��buf
            }
        }
        _argv[argv_cnt] = (char *)0;  //������־
        if (fork() == 0) {
            exec(_argv[0], _argv);
        } else {
            wait((int *)0);
        }
    }
    exit(0);
}