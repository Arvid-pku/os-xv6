#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

int main(int argc, char *argv[])
{
    int fdp[2];
    int fds[2];
    pipe(fdp);
    pipe(fds);
    if(fork() == 0)
    {
        char* f = (char*)malloc(1);
        f[0] = 'h';
        int fwrite = dup(fds[1]);
        int fread = dup(fdp[0]);
        close(fdp[0]);
        close(fdp[1]);
        close(fds[0]);
        close(fds[1]);
        if(read(fread, f, 1))
        {
            printf("%d: received ping\n", getpid());
        }
        write(fwrite, f, 1);
        free(f);
    }
    else
    {
        char* f = (char*)malloc(1);
        f[0] = 'h';
        int fwrite = dup(fdp[1]);
        int fread = dup(fds[0]);
        close(fdp[0]);
        close(fdp[1]);
        close(fds[0]);
        close(fds[1]);
        write(fwrite, f, 1);
        if(read(fread, f, 1))
        {
            printf("%d: received pong\n", getpid());
        }
        wait((int *)0);
        free(f);
    }
    exit(0);
}
