#include <pthread.h>
#include <semaphore.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>

#define CHILDREN 12
sem_t father;
pthread_mutex_t mother;
int value, n;

void *motherFunc()
{   
    pthread_mutex_lock(&mother);
    for (int i = 0; i < CHILDREN; i++) {
        printf("Child #%d is woken up by mother.\n", i);
        usleep(100);
    }
    for (int i = 0; i < CHILDREN; i++) {
        printf("Child #%d is fed breakfast by mother.\n", i);
        usleep(100);
    }
    for (int i = 0; i < CHILDREN; i++) {
        printf("Child #%d is sent to school by mother.\n", i);
        usleep(100);
    }
    for (int i = 0; i < CHILDREN; i++) {
        printf("Child #%d is given dinner by mother.\n", i);
        usleep(100);
    }
    for (int i = 0; i < CHILDREN; i++) {
        printf("Child #%d is given a bath by mother.\n", i);
        usleep(100);
    }
    sem_post(&father);
    
    printf("Mother is going to sleep and waking up Father to take care of the children.\n");
    pthread_mutex_lock(&mother);

    pthread_exit(NULL); 
}

void *fatherFunc()
{   
    sem_wait(&father);
     for (int i = 0; i < CHILDREN; i++) {
        printf("Child #%d is read a book by father.\n", i);
    }
    for (int i = 0; i < CHILDREN; i++) {
        printf("Child #%d is tucked in bed by father.\n", i);
    }
    printf("Father is going to sleep and waking up Mother to take care of the children.\n");
    pthread_mutex_unlock(&mother);

    pthread_exit(NULL); 
}

int main(int argc, char *argv[])
{   
    pthread_t m, f;
    pthread_mutex_init(&mother, NULL);
    pthread_mutex_unlock(&mother);
    sem_init(&father,0,0);
    n = atoi(argv[1]);
    value = 0;

    pthread_create(&m, NULL, (void *)motherFunc, NULL);
    pthread_create(&f, NULL, (void *)fatherFunc, NULL);
    pthread_join(m, NULL);
    pthread_join(f, NULL);

    pthread_mutex_destroy(&mother);
    sem_destroy(&father);

    return 0;
}
