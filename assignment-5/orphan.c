#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>

int main(){
    int id;
    id = fork();
    if(id == 0){
        //sleep(10);
        printf("I am child and have PID = %d\n", getpid());
        printf("My parent have PPID = %d\n", getppid());
        printf("i am orphan\n");
        sleep(10);
        printf("Now My parent is : %d", getppid());
    }
    else{
        printf("I am parent and have PID = %d\n", getpid());
        printf("My child have PID = %d\n", id);
        sleep(2);
        printf("Parent killed\n");
    }
    return 0;
}
