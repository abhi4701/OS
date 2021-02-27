#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

int sum = 0;

void *cal_sum(void *num1) {
	int *num, i;
	num = (int *)num1;

	sum = ((*num) * (*num + 1)) / 2;

	pthread_exit(NULL);
}

// to compile this file cmd: gcc -g -pthread filename.c -o exename 

int main() {
	int i, num;
	pthread_t id;
	printf("Enter the number: ");
	scanf("%d", &num);
	pthread_create(&id, NULL, cal_sum, (void *)&num);
	// can perform any other function here.
	pthread_join(id, NULL);
	printf("The sum of n natural numbers is : %d \n", sum);
	return 0;
}
