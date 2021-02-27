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
char lineChange[5] = "\n";

void *writer(void *wno) {
	sem_wait(&wrt);
	// sleep(1);
	filePointer = fopen("test.txt", "a");
	printf("Writer %d Enter Data to be written :", *((int *)wno));
	scanf("%s", dataToBeWritten);
	fprintf(filePointer, "%s\n", dataToBeWritten);
	fclose(filePointer);
	// write(fd, lineChange, sizeof(lineChange));
	sem_post(&wrt);
}

void *reader(void *rno) {
	pthread_mutex_lock(&mutex);
	char ch;
	FILE *fp;
	readerCount++;
	if (readerCount == 1) {
	
		sem_wait(&wrt);
	}
	pthread_mutex_unlock(&mutex);
	fp = fopen("test.txt", "r");
	// fscanf(fp, "%s", dataToBeRead);
	// fgets(dataToBeRead, 100, (FILE *)fp);
	printf("Data Written In File is:\n");
	while(1){
		ch = getc(fp);
		if(ch==EOF)
			break;
		printf("%c", ch);
	}
	fclose(fp);
	pthread_mutex_lock(&mutex);
	readerCount--;
	if (readerCount == 0) {
	
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

	int r = 10, w = 2;
	pthread_mutex_init(&mutex, NULL);
	sem_init(&wrt, 0, 1);
	int a[w], b[r];
	int i, j;
	for(i = 0; i < w; i++)
	{
		a[i] = i + 1;
	}

	for(i = 0; i < r; i++) {
		b[i] = i + 1;
	}
	filePointer = fopen("test.txt", "w");
	fclose(filePointer);
	// for (i = 0; i < w; i++) {
	// 	pthread_create(&writerthreads[i], NULL, (void *)writer, (void *)&a[i]);
	// }
	// for (i = 0; i < r; i++) {
	// 	pthread_create(&readerthreads[i], NULL, (void *)reader, (void *)&b[i]);
	// }
	for(i = 0; i < w; i++){
		pthread_create(&writerthreads[i], NULL, (void *)writer, (void *)&a[i]);
		sleep(5);
		for(j=0; j < r/w; j++){
			pthread_create(&readerthreads[i], NULL, (void *)reader, (void *)&b[i]);
			sleep(1);
		}
		//sleep(4);
	}
	
	for (i = 0; i < r; i++) {
		pthread_join(readerthreads[i], NULL);
	}

	for (i = 0; i < w; i++) {
		pthread_join(writerthreads[i], NULL);
	
		
	}

	pthread_mutex_destroy(&mutex);
	sem_destroy(&wrt);
	// fscanf(filePointer, "%s", dataToBeRead);
	// printf("Final Data Written Was: %s\n", dataToBeRead);
	// close(fd);
	// fclose(filePointer);


	return 0;
}
