#include<stdio.h>
#include<stdlib.h>
#include<signal.h>
#include<unistd.h>

void InterceptFunction(int signalNum)
{
	printf("\tStarting Work(Signal Caught: %d)!\n", signalNum);
	// exit(0);
}

int isLeapYear(int n){
	if(n%4==0 && (n%100!=0 || n%400==0))
		return 1;
	return 0;
}

int main()
{
	int c = 1, n;
	int pid = fork();
	if(pid==0){
		while(1){
			signal(SIGALRM, InterceptFunction);
			pause();
			printf("\t\tEnter the year: ");
			scanf("%d", &n);
			if(isLeapYear(n))
				printf("\t\t%d is a Leap Year.\n", n);
			else
				printf("\t\t%d is not a Leap Year.\n", n);
			
		}
	}
	else{
		while(1)
		{
			printf("=====================\nResume work Kid!\n");
			kill(pid, SIGALRM);
			sleep(5);
		}
	}
	
	return 0;
}
