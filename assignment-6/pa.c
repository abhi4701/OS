#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>

int main(){
    int fd[2];
    // fd[1] = write;
    // fd[0] = read;

    char glob[50] = "Char";
    if(pipe(fd) == -1){
        printf("Error\n");
    }
    char msg[50];
    int id = fork();
    if(id == 0){
        printf("I am chlid: ppid -> %d and pid -> %d\n", getppid(), getpid());
        close(fd[0]);
        int x;
        printf("Enter a message: \n");
        scanf("%[^\n]%*c", msg);
        write(fd[1], msg, sizeof(msg));
        close(fd[1]);
    }
    else{
        printf("I am parent: chlidId -> %d and pid -> %d\n", id, getpid());
        close(fd[1]);
        char got[50];
        read(fd[0], got, sizeof(got));
        close(fd[0]);
        strcat(glob, got);
        printf("Got from chlid process: %s\n", got);
        
    }

    return 0;
}
