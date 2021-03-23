#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

int main(int argc, char *argv[])
{
    if(argc != 2)
    {
        write(2, "ERROR: Please input one num as sleeping time.\n", 47);
        exit(0);
    }
    int len = 0;
    char *s = argv[1];
    while('0' <= *(s) && *(s) <= '9')
    {
        len++;
        s++;
    }
    if(len != strlen(argv[1]))
    {
        write(2, "ERROR: Please input num.\n", 26);
        exit(0);
    }
    int sleeptime = atoi(argv[1]);
    sleep(sleeptime);
    exit(0);
}
