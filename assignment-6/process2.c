#include <stdio.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>
#include <string.h>


int main(){
    printf("Process 2\n");
    char str[20][50000];

    int fd = open("test", O_RDONLY);

    if(fd == -1){
        printf("Error\n");
    }
    int i;
    for(int i = 0; i < 2; i++){
      read(fd, str + i, sizeof(str[i]));
      printf("Copied form process1\n");
    }
    close(fd);
    (strcmp(str[0], str[1]) == 0) ? printf("Same\n") : printf("Not Same\n");
    return 0;
}
