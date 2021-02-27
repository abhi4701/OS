#include <pthread.h>
#include <semaphore.h>
#include <stdlib.h>
#include <stdio.h>

#define MaxItems 3
#define BufferSize 2
sem_t empty;
sem_t full;
int in = 0;
int out = 0;
int buffer[BufferSize];
pthread_mutex_t mutex;

void *producer(void *pno) {
    int item;
    for (int i = 0; i < MaxItems; i++) {
        item = rand() % 10;
        sem_wait(&empty);
        pthread_mutex_lock(&mutex);
        buffer[in] = item;
        printf("Produced %d: Inserted product %d in buffer %d\n", *((int *)pno), buffer[in], in);
        in = (in + 1) % BufferSize;
        pthread_mutex_unlock(&mutex);
        sem_post(&full);
    }
}
void *consumer(void *cno) {
    for (int i = 0; i < MaxItems; i++) {
        sem_wait(&full);
        pthread_mutex_lock(&mutex);
        int item = buffer[out];
        printf("Consumed %d: Removed product %d from buffer %d\n", *((int *)cno), item, out);
        out = (out + 1) % BufferSize;
        pthread_mutex_unlock(&mutex);
        sem_post(&empty);
    }
}

int main() {

    pthread_t prod[3], consume[3];
    pthread_mutex_init(&mutex, NULL);
    sem_init(&empty, 0, BufferSize);
    sem_init(&full, 0, 0);

    int a[5] = {1, 2, 3};

    for (int i = 0; i < 3; i++) {
        pthread_create(&prod[i], NULL, (void *)producer, (void *)&a[i]);
    }
    for (int i = 0; i < 3; i++) {
        pthread_create(&consume[i], NULL, (void *)consumer, (void *)&a[i]);
    }

    for (int i = 0; i < 3; i++) {
        pthread_join(prod[i], NULL);
    }
    for (int i = 0; i < 3; i++) {
        pthread_join(consume[i], NULL);
    }

    pthread_mutex_destroy(&mutex);
    sem_destroy(&empty);
    sem_destroy(&full);

    return 0;
}
