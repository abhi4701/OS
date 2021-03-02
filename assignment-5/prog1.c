#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
int main()
{
    int id;
    printf("child id: %d of parent id: %d \n", getpid(), getppid());
    id = fork();
    if (id == 0)
    {
        printf("pid and ppid of child is: %d %d \n", getpid(), getppid());
        sleep(6);
    }
    else
    {
        printf("pid and ppid of parent is: %d %d \n", getpid(), getppid());
        sleep(6);
    }

    return 0;
}
