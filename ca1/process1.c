#include <stdio.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <string.h>
#include <fcntl.h>

int main(){
    printf("Process 1\n");
    char str[2][50];

    printf("Enter the strings\n");
    scanf("%[^\n]%*c", str[0]);
    scanf("%[^\n]%*c", str[1]);

    int fd = open("test", O_WRONLY);
    if(fd == -1){
        printf("Error\n");
    }
    int i;
    for(int i = 0; i < 2; i++){
        write(fd, str + i, sizeof(str[i]));
        printf("Got Copied: %s\n", str[i]);
    }
    close(fd);
  
    return 0;
}
