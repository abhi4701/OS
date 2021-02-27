#include<stdio.h>
#include<stdlib.h>
#include<semaphore.h>
#include<pthread.h>

sem_t sem1;

void *thread1(void * args) {
	/**
	 * sem_wait(): same as wait; it checks if the resouce is free or not
	 * if value is 1 then it this thread can run its critical condition
	 * if value is 0 this wait function will never end
	 */
	sem_wait(&sem1); 
	printf("ba"); //critical section
	/**
	* sem_post(): this will increment the value of sem1 again to 1 
	* and after this thread 2 can execute its critical part
	*/
	sem_post(&sem1);
}

void *thread2(void * args) {
	sem_wait(&sem1);
	printf("ab");  //critical section
	sem_post(&sem1);
}

int main() {
	pthread_t t1;
	pthread_t t2;
	int i =3;
	while(i>0) {				
		sem_init(&sem1, 0, 1); //initialization of samaphore variable

		pthread_create(&t1, NULL, &thread1, NULL);
		pthread_create(&t2, NULL, &thread2, NULL);

		pthread_join(t1, NULL);
		pthread_join(t2, NULL);

		//free the recources accociated with semaphore
		sem_destroy(&sem1);
		i -= 1;
	}
	printf("\n");
	return 0;
}
