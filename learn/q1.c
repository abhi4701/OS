#include<stdio.h>
#include<stdlib.h>
#include<signal.h>
#include <unistd.h>

void exitFunction(int signalNum)
{
	printf("\nNo more, PLEASE!(Signal Caught: %d).\n", signalNum);
	// exit(0);
}

int main()
{
	int c = 1;
	signal(SIGINT, exitFunction);
	while(1)
	{
		printf("%3d little Sheep go Bah! Bah! Bah!\n", c);
		c++;
		sleep(1);
	}
	return 0;
}
