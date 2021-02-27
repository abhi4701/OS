#include <stdio.h>
#include <signal.h>
#include <unistd.h>

void fun(int sig_no)
{
	printf("\nSignal number of send by parent is : %d\n", sig_no);
	int year, y;
	printf("\nInput the year : ");
	scanf("%d", &year);
	
	y = year;
	if(y%100 == 0)
		y /= 100;

	if(y%4 == 0)
		printf("%d is a leap year\n", year);
	else
		printf("%d is not a leap year\n", year);
}

int main()
{
	int pid = fork();
	if(pid == 0)
	{
		while(1)
		{
		    signal(SIGALRM, fun);
		}
	}
	else
	{
		while(1)
		{
			sleep(5);
			printf("\nInside Parent\n");
			kill(pid, SIGALRM);
			printf("\nsignal is send to child\n");
		}
	}
}
