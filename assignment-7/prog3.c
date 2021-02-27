#include <stdio.h>
#include <signal.h>
#include <unistd.h>

void function(int sig){
    printf("Signal Number %d \n",sig);
}

int checkYear(int year) {
    if (year % 400 == 0)
        return 1;
 
    if (year % 100 == 0)
        return 0;
 
    if (year % 4 == 0)
        return 1;
    return 0;
}

int main() {
	int pid = fork();
	if(pid == -1) {
		printf("ERROR!!\n");
		return 1;
	}
	int year;
	if(pid == 0) {
		while(1) {
			signal(SIGALRM, function);
			pause();
			printf("Enter the year: ");
			scanf("%d", &year);
			if(checkYear(year)) {
				printf("%d is a leap Year\n", year);
			} else {
				printf("%d is not a leap Year\n", year);
			}
		}
	} else {
		while(1) {
			printf("\nsignal is send to child\n");
			sleep(5);
			kill(pid, SIGALRM);
		}
	}
}
