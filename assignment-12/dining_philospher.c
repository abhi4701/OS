#include <pthread.h>
#include <semaphore.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/wait.h>


#define N 5 
#define THINKING 2 
#define HUNGRY 1 
#define EATING 0 
#define LEFT (philosopher + 4) % N 
#define RIGHT (philosopher + 1) % N 

int action[N]; 
int phil[N] = { 0, 1, 2, 3, 4 }; 

sem_t mutex; 
sem_t S[N]; 

void test(int philosopher)  { 
	if (action[philosopher] == HUNGRY && action[LEFT] != EATING && action[RIGHT] != EATING) { 
		// action that eating 
		action[philosopher] = EATING; 
		sleep(2); 
		printf("Philosopher %d takes fork %d and %d\n", philosopher + 1, LEFT + 1, philosopher + 1); 
		printf("Philosopher %d is Eating\n", philosopher + 1); 
		sem_post(&S[philosopher]); 
	} 
} 

// pick up chopsticks 
void take_fork(int philosopher) 
{ 

	sem_wait(&mutex); 

	// hungry state 
	action[philosopher] = HUNGRY; 

	printf("Philosopher %d is Hungry\n", philosopher + 1); 

	// eat if ok 
	test(philosopher); 

	sem_post(&mutex); 

	// if unable to eat wait
	sem_wait(&S[philosopher]); 

	sleep(1); 
} 

// put down chopsticks 
void put_fork(int philosopher){  

	sem_wait(&mutex); 

	// thinking 
	action[philosopher] = THINKING; 

	printf("Philosopher %d putting fork %d and %d down\n", philosopher + 1, LEFT + 1, philosopher + 1); 
	printf("Philosopher %d is thinking\n", philosopher + 1); 

	test(LEFT); 
	test(RIGHT); 

	sem_post(&mutex); 
} 

// representing philospher
void* philospher(void* num) 
{ 
    int n = 3;
	while (n--) {
        // printf("Round: %d\n", n);
		int* i = num; 
		sleep(1); 
		take_fork(*i); 
		sleep(0); 
		put_fork(*i); 
	} 
} 

int main() 
{ 

	int i; 
	pthread_t thread_id[N]; 

	// initialize the semaphores 
	sem_init(&mutex, 0, 1); 

	for (i = 0; i < N; i++) 
        sem_init(&S[i], 0, 0); 

	for (i = 0; i < N; i++) { 
        printf("----------------------\n");
		// create philosopher thread
		pthread_create(&thread_id[i], NULL, philospher, &phil[i]); 

		printf("Philosopher %d is thinking\n", i + 1); 

	} 

	for (i = 0; i < N; i++) {
        pthread_join(thread_id[i], NULL); 
        printf("----------------------\n"); 
    }
} 
