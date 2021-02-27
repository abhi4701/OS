#include <pthread.h>
#include <semaphore.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <fcntl.h>

sem_t wrt;
pthread_t writerthreads[100], readerthreads[100];
pthread_mutex_t mutex;
int data = 1;
int readerCount = 0;
int fd; 
FILE *filePointer;
char file_name[100];
char dataToBeRead[100];
char dataToBeWritten[100];

void *writer(void *wno) {
	sem_wait(&wrt);
	sleep(1);
	filePointer = fopen("test.txt", "w");
	printf("Writer %d Enter Data to be written :", *((int *)wno));
	scanf("%s", dataToBeWritten);
	write(fd, dataToBeWritten, sizeof(dataToBeWritten));
	/*
	if (strlen(dataToBeWritten) > 0)
	{
		fputs(dataToBeWritten, filePointer);
		fputs("\n", filePointer);
	}*/
	sem_post(&wrt);
}

void *reader(void *rno) {
	pthread_mutex_lock(&mutex);
	readerCount++;
	if (readerCount == 1) {
	
		sem_wait(&wrt);
	}
	lseek(fd,0, SEEK_SET); // helps in reading form first data bit;
	read(fd, dataToBeRead, sizeof(dataToBeWritten));
	/*
	//filePointer = fopen("test.txt", "r");
	pthread_mutex_unlock(&mutex);
	while (fgets(dataToBeRead, strlen(dataToBeRead), filePointer) != NULL){
	
		printf("Reader %d: read data as %s\n", *((int *)rno), dataToBeRead);
	}
	*/
	pthread_mutex_lock(&mutex);
	//fclose(filePointer);
	readerCount--;
	if (readerCount == 0){
	
		sem_post(&wrt);
	}
	pthread_mutex_unlock(&mutex);
}

/**
 * @::Constrain: Only for same Data
 * R - W => Problem 
 * W - R => Problem 
 * W - W => Problem 
 * @::Constrain: for any number of Data
 * R - R => !Problem
*/

int main(){

	int r = 10, w= 5;
	pthread_mutex_init(&mutex, NULL);
	sem_init(&wrt, 0, 1);
	int a[r], b[w];
	int i = 0;
	for(i = 0; i < r; i++) {
		a[i] = i + 1;
	}

	for (i = 0; i < w; i++) {
		b[i] = i + 1;
	}
	fd = open("test.txt", O_RDWR);
	for (i = 0; i < w; i++) {
	
		pthread_create(&writerthreads[i], NULL, (void *)writer, (void *)&b[i]);
	}
	for (i = 0; i < r; i++) {
	
		pthread_create(&readerthreads[i], NULL, (void *)reader, (void *)&a[i]);
	}

	for (i = 0; i < r; i++) {
	
		pthread_join(readerthreads[i], NULL);
	}

	for (i = 0; i < w; i++) {
	
		pthread_join(writerthreads[i], NULL);
	}
	//fclose(filePointer);
	close(fd);
	pthread_mutex_destroy(&mutex);
	sem_destroy(&wrt);
	return 0;
}
