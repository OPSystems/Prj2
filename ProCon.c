#include <pthread.h>
#include <semaphore.h>
#include <stdlib.h>
#include <stdio.h>

#define MAX_SIZE 5 
pthread_mutex_t bufLock;
sem_t empty, full;

void *producer(char* buf)
{   
    int in = 0;
    for(int i = 0; i < MAX_SIZE; i++) {
        int item = rand()%(10-0) + 0;
        sem_wait(&full);
        pthread_mutex_lock(&bufLock);
        buf[in] = item;
        printf("Producer inserted %d at index %d\n", buf[in],in);
        in = (in+1) % MAX_SIZE;
        pthread_mutex_unlock(&bufLock);
        sem_post(&empty);
    }
}

void *consumer(char* buf)
{   
    int out = 0;
    for(int i = 0; i < MAX_SIZE; i++) {
        sem_wait(&empty);
        pthread_mutex_lock(&bufLock);
        int item = buf[out];
        printf("Consumer removed %d from index %d\n",item, out);
        out = (out+1) % MAX_SIZE;
        pthread_mutex_unlock(&bufLock);
        sem_post(&full);
    }
}

int main()
{   
    srand(time(NULL));
    int buffer[MAX_SIZE];
    pthread_t p;
    pthread_mutex_init(&bufLock, NULL);
    sem_init(&full,0,MAX_SIZE);
    sem_init(&empty,0,0);

    pthread_create(&p, NULL, (void *)producer, &buffer);
    consumer(&buffer);

    return 0;
}