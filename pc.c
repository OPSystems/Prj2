#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>
#include <time.h>

#define MAX_SIZE 5
#define NUM_ITER MAX_SIZE*1
sem_t empty, full;

void useChar(char c) {
    printf("Character %c has been consumed\n", c);
}

char getChar() {
    char c;
    c = 'A' + (rand() % 26);
    printf("Character %c has been produced\n", c);
    return c;
}

void producer (char* buf) {
    int in = 0;
    int count = 0;
    for(;;) {
        sem_wait(&full);
        count++;
        buf[in] = getChar();

        in = (in + 1) % MAX_SIZE;
        sem_post(&empty);

        int value;
        sem_getvalue(&empty, &value);
        printf("Signaling empty: %d\n", value);
    }
}

void consumer(char* buf) {
    int out = 0;
    int count = 0;
    for(;;) {
        sem_wait(&empty);
        count++;
        useChar(buf[out]);

        out = (out+1) % MAX_SIZE;
        sem_post(&full);

        int value;
        sem_getvalue(&full, &value);
        printf("Signaling full: %d\n", value);
    }
}


int main() {
    srand(time(0));

    char buffer[MAX_SIZE];
    pthread_t p1;
    pthread_t p2;
    sem_init(&full, 0, MAX_SIZE);
    sem_init(&empty, 0, 0);
    
    printf("Producer Started\n");
    pthread_create(&p1, NULL, (void*)producer, &buffer);
    printf("Consumer Started\n");
    pthread_create(&p2, NULL, (void*)consumer, &buffer);

    pthread_join(p1, NULL);
    pthread_join(p2, NULL);
    

    return 0;
}