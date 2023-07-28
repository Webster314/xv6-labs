#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
#include "kernel/fs.h"
char*
fmtname(char *path)
{
    static char buf[DIRSIZ+1];
    char *p;
    for(p=path+strlen(path); p >= path && *p != '/'; p--)
        ;
    p++;
    if(strlen(p) >= DIRSIZ)
        return p;
    memmove(buf, p, strlen(p));
    memset(buf+strlen(p), 0, DIRSIZ-strlen(p));
    return buf;
}

void
find(char * path, char * nm){
    int fd;
    char buf[512];
    struct dirent de;
    struct stat st;
    // open path
    if((fd = open(path, 0)) < 0){
        fprintf(2, "find: cannot open %s\n", path);
        return;
    }
    if(fstat(fd, &st) < 0){
        fprintf(2, "find: cannot stat %s\n", path);
        close(fd);
        return;
    }

    if(strcmp(fmtname(path), nm) == 0){
        printf("%s\n", path);
    }
    if(st.type == T_DIR){
        if(strlen(path) + 1 + DIRSIZ + 1 > sizeof buf){
            printf("find: path too long\n");
            close(fd);
            return;
        }
        while(read(fd, &de, sizeof(de)) == sizeof(de)){
            if(de.inum == 0 || strcmp(de.name, ".") == 0 || strcmp(de.name, "..") == 0){
                continue;
            }
            strcpy(buf, path);
            char * p = buf+strlen(path);
            *p++ = '/';
            strcpy(p, de.name);
            find(buf, nm);
        }
    }
    close(fd);
}

int main(int argc, char * argv[]){
    if(argc == 1){
        find(".", "");
    }
    if(argc == 2){
        find(".", argv[1]);
    }
    for(int i = 2; i < argc; i++){
        find(argv[1], argv[i]);
    }
    exit(0);
}