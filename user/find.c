#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
#include "kernel/fs.h"



void find(char *path, char *name)
{
    char buf[512], *p;
    int fd;
    struct dirent de;
    struct stat st;

    if((fd = open(path, 0)) < 0)
    {
        fprintf(2, "find: cannot open %s\n", path);
        return;
    }

    if(fstat(fd, &st) < 0)
    {
        fprintf(2, "find: cannot stat %s\n", path);
        close(fd);
        return;
    }

    switch(st.type)
    {
    case T_FILE:
        fprintf(2, "find: %s is not a dir\n", path);
        break;

    case T_DIR:
        if(strlen(path) + 1 + DIRSIZ + 1 > sizeof buf)
        {
            printf("find: path too long\n");
            break;
        }
        strcpy(buf, path);
        p = buf+strlen(buf);
        *p++ = '/';
        while(read(fd, &de, sizeof(de)) == sizeof(de))
        {
            if(de.inum == 0 || strcmp(de.name, ".")==0||strcmp(de.name, "..")==0)
                continue;
            memmove(p, de.name, DIRSIZ);
            p[DIRSIZ] = 0;
            if(stat(buf, &st) < 0)
            {
                printf("find: cannot stat %s\n", buf);
                continue;
            }
            switch(st.type)
            {
            case T_FILE:
                if(strcmp(p, name) == 0)
                {
                    printf("%s\n", buf);
                }
                break;
            case T_DIR:
                find(buf, name);
                break;

            }
        }
        break;
    }
    close(fd);
}



int main(int argc, char *argv[])
{
    char *dir;
    char *name;
    switch (argc)
    {
    case 2:
        dir = ".";
        name = argv[1];
        break;
    case 3:
        dir = argv[1];
        name = argv[2];
        break;
    default:
        fprintf(2, "ERROR: Wrong number of parameters./n");
        exit(0);
    }
    if((open(dir, 0)) < 0){
        fprintf(2, "ls: cannot open %s\n", dir);
        exit(0);
    }
    find(dir, name);
    
    exit(0);
}