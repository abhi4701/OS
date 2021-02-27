#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <pthread.h>
#include <sys/types.h>
#include <sys/syscall.h>

#define gettid() ((pid_t)syscall(SYS_gettid))

int  isPrime(int n) { 

    if (n <= 1)  
		return 0; 
    if (n <= 3)  
		return 1; 
  
    if (n%2 == 0 || n%3 == 0) 
		return 0; 
  
    for (int i=5; i*i<=n; i=i+6) 
        if (n%i == 0 || n%(i+2) == 0) 
           return 0; 
  
    return 1; 
} 

void *solver(void *args) {
	int *num;
	num = (int *)args;
	if (*num <= 0) {
		printf("Enter a positive number\n");
	}
	int ok = isPrime(*num);
	if(ok) {
		printf("%d is a prime number\n", *num);
	} else {
		printf("%d is not a prime number\n", *num);
	}
}

int main() {
	pthread_t thread;
	int true = 1;
	printf("%d main method pid\n", getpid());
	printf("%d main method tid\n", gettid());
	if(getpid() == gettid()) {
		printf("This is a main thread");
	}
	while (true) {
		int args;
		printf("\nEnter a number: ");
		scanf("%d", &args);
		pthread_create(&thread, NULL, &solver,(void *)&args);
		pthread_join(thread, NULL);
		sleep(1);
	}
	return 0;
}
