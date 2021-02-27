#include<stdio.h>
#include<stdlib.h>
#include<signal.h>
#include<unistd.h>

void InterceptFunction(int signalNum)
{
	printf("Okay, Start(Signal Caught: %d)!\n", signalNum);
	// exit(0);
}

int main()
{
	int c = 1;
	int pid = fork();
	if(pid==0){
		sleep(5);
		kill(getppid(), SIGALRM);
		exit(0);
	}
	else{
		signal(SIGALRM, InterceptFunction);
		pause();
		while(1)
		{
			printf("%3d little Sheep go Bah! Bah! Bah!\n", c);
			c++;
			sleep(1);
		}
	}
	
	return 0;
}
