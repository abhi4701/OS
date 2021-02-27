#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>


int main(){
    int id;
    id = fork();
    
    //Child process
    if(id == 0){
        sleep(2);
        printf("I am child and have PID = %d\n", getpid());
        printf("My parent have PPID = %d\n", getppid());
        printf("Child Died!!!!\n");
    }
    //Parent 
    else{
        sleep(10);
        printf("I am parent and have PPID = %d\n", getpid());
        printf("My child have ID = %d\n", id);
        printf("I am ZOMBIE now !!!!!\n");
    }

}
