#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

void goprocess(int fread)
{
    int myout = 0;
    int son = 0;
    int nextp[2];
    pipe(nextp);
    int i = 0;
    while (read(fread, &i, sizeof(int)))
    {
        if(myout == 0)
        {
            printf("prime %d\n", i);
            myout = i;
        }
        else
        {
            if(son == 0)
            {
                if(fork() == 0)
                {
                    close(nextp[1]);
                    goprocess(nextp[0]);
                }
                else
                {
                    son = 1;
                    close(nextp[0]);
                    write(nextp[1], &i, sizeof(i));
                }
                
            }
            else if(i % myout != 0)
            {
                write(nextp[1], &i, sizeof(i));
            }
        }
    }
    if(nextp[1])
    {
        close(nextp[1]);
        wait(0);
    }
    
}





int main(int argc, char *argv[])
{
    int p[2];
    pipe(p);
    if(fork() == 0)
    {
        close(p[1]);
        goprocess(p[0]);
    }
    else
    {
        close(p[0]);
        for(int i = 2; i <= 35; i++)
        {
            write(p[1], &i, sizeof(int));
        }
        close(p[1]);
        wait(0);
    }
    exit(0);
}
