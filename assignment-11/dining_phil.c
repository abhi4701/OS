#include<stdio.h>
#include<stdlib.h>
#include<pthread.h>
#include<semaphore.h>
#include<unistd.h>

int n = 5;
sem_t room;
sem_t chopstick[5];

void eat(int phil)
{
	printf("\nPhilosopher %d is eating",phil);
}

void * philosopher(void * num)
{
	int phil=*(int *)num;

	sem_wait(&room);
	printf("\nPhilosopher %d has entered room",phil);
	sem_wait(&chopstick[phil]);
	sem_wait(&chopstick[(phil+1)%n]);

	eat(phil);
	sleep(2);
	printf("\nPhilosopher %d has finished eating",phil);

	sem_post(&chopstick[(phil+1)%n]);
	sem_post(&chopstick[phil]);
	sem_post(&room);
}




int main()
{
	int i,a[n];
	pthread_t tid[n];
	
	sem_init(&room,0,4);
	
	for(i=0;i<n;i++)
		sem_init(&chopstick[i],0,1);
		
	for(i=0;i<n;i++){
		a[i]=i;
		pthread_create(&tid[i],NULL, philosopher,(void *)&a[i]);
	}
	for(i=0;i<n;i++)
		pthread_join(tid[i],NULL);
}

