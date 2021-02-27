#include <stdio.h>
#include <signal.h>
#include <unistd.h>
#include <stdlib.h>

void fun1(int sig) {
	//to handle the signal
	int year;
	printf("Signal number: %d\n", sig);
	printf("Enter the year\n");
	scanf("%d",&year);
	if (year<=0){
		printf("Invalid Year\n");
	}
	else if (year % 400 == 0 )
		printf( "%d is leap year.\n",year);
	else if (year % 4 == 0 ){
		if (year % 100 == 0 )
			printf( "%d is not a leap year.\n",year);
		else
			printf( "%d is leap year.\n",year);
	}
	else
		printf( "%d is not a leap year.\n",year);
}

int main() {
  int p = fork();
  if(p == 0) {
    int i=0;
    signal(SIGALRM, fun1);
  }
  else {
    while(1){
      sleep(5);
      kill(p, SIGALRM);
    }
  }
  return 0;
}
		

