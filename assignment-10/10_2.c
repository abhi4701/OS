#include <semaphore.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <pthread.h>
#define BufferSize 3
#include <unistd.h>
int buffer[BufferSize];
sem_t full, empty;
pthread_mutex_t mutex;
int in = 0, out = 0, item_in, item_out;
void *producerFn(void *pno)
{
        sem_wait(&empty);
        pthread_mutex_lock(&mutex);
        printf("Name a product(-9999 to stop Production) :");
        scanf("%d", &item_in);
        buffer[in] = item_in;
        in = (in + 1) % BufferSize;
        pthread_mutex_unlock(&mutex);
        sem_post(&full);
}
void *consumerFn(void *cno)
{
        printf("buffer[out]%d\n", buffer[out]);
        sem_wait(&full);
        pthread_mutex_lock(&mutex);
        item_out = buffer[out];
        out = (out + 1) % BufferSize;
        pthread_mutex_unlock(&mutex);
        sem_post(&empty);
}
int main()
{
        int i = 0, k = 0;
        pthread_t producer[3], consumer[3];
        pthread_mutex_init(&mutex, NULL);
        sem_init(&empty, 0, BufferSize);
        sem_init(&full, 0, 0);
        do
        {
                printf("Name a product(-9999 to stop Production) :");
                scanf("%d", &buffer[i++]); //
        } while (i < BufferSize && buffer[i - 1] != -9999);
        if (buffer[i - 1] != -9999)
        {
                do
                {
                        pthread_create(&consumer[k], NULL, (void *)consumerFn, NULL);
                        sleep(2);
                        pthread_create(&producer[k], NULL, (void *)producerFn, NULL);
                        pthread_join(producer[k], NULL);
                        pthread_join(consumer[k], NULL);
                        k++;
                } while (item_in != -9999);
        }
        while (item_out != -9999)
        {
                pthread_create(&consumer[k], NULL, (void *)consumerFn, NULL);
                k++;
        }
        pthread_mutex_destroy(&mutex);
        sem_destroy(&empty);
        sem_destroy(&full);
        return 0;
}
