#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
#include "kernel/fs.h"

char *getname(char *path) {
    char *p;
    // �ҵ��ļ����ķ����ܼ򵥣������һ��/����
    for (p = path + strlen(path); p >= path && *p != '/'; p--);
    p++;
    return p; //����ĩβ���ļ���
}

void find(char *directory, char *filename) {
    char buf[512], *p;
    int fd;
    struct dirent de;
    struct stat st;

    if ((fd = open(directory, 0)) < 0) {
        fprintf(2, "ls: cannot open %s\n", directory);
        return;
    }

    if (fstat(fd, &st) < 0) {
        fprintf(2, "ls: cannot stat %s\n", directory);
        close(fd);
        return;
    }
    switch (st.type) { //�鿴·������
        case T_FILE: //���ļ���ƥ���ļ���
            if (strcmp(getname(directory), filename) == 0) { //�Ƚ�·��ĩβ�ļ����Ͳ��ҵ��ļ���
                printf("%s \n", directory);
            }
            break;
        case T_DIR: //��Ŀ¼���ų�.��..�ݹ������
            if (strlen(directory) + 1 + DIRSIZ + 1 > sizeof buf) {
                printf("find: path is too long \n");
                break;
            }
            strcpy(buf, directory);
            p = buf + strlen(buf);
            *p++ = '/'; //ƴ�ӳ���Ŀ¼
            while (read(fd, &de, sizeof(de)) == sizeof(de)) { //����Ŀ¼
                if (de.inum == 0 || strcmp(".", de.name) == 0 ||
                    strcmp("..", de.name) == 0) {
                    continue;
                }
                memmove(p, de.name, DIRSIZ); //ƴ�յ�ǰ�ļ�·��
                p[DIRSIZ] = 0;
                if (stat(buf, &st) < 0) {
                    printf("ls: cannot stat %s\n", buf);
                    continue;
                }
                find(buf, filename); //����find�ݹ����
            }
            break;
    }
    close(fd);
}

int main(int argc, char *argv[]) {
    if (argc != 3) {
        fprintf(2, "Usage: find <directory> <filename>\n");
        exit(1);
    }
    find(argv[1], argv[2]);
    exit(0);
}