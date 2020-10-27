#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>
#include <time.h>
#include <unistd.h>



sem_t goMother, goFather;
int num_times = 2;
int day = 0;

void mother() {

    for(;;) {
        sem_wait(&goMother);
        day++;
        printf("This is day #%d in the life of Mother Hubbard\n\n", day);

        for (int i = 1; i <= 12; i++) {
            printf("Child #%d is woken up by Mother\n", i);
            usleep(100);
        }

        printf("\n");

        for (int i = 1; i <= 12; i++) {
            printf("Child #%d is fed breakfast by Mother\n", i);
            usleep(100);
        }

        printf("\n");

        for (int i = 1; i <= 12; i++) {
            printf("Child #%d is sent to school by Mother\n", i);
            usleep(100);
        }

        printf("\n");

        for (int i = 1; i <= 12; i++) {
            printf("Child #%d is given dinner by Mother\n", i);
            usleep(100);
        }

        printf("\n");

        for (int i = 1; i <= 12; i++) {
            printf("Child #%d is given a bath by Mother\n", i);
            sem_post(&goFather);
            // int value;
            // sem_getvalue(&goFather, &value);
            // printf("Signaling Father: %d\n", value);
            usleep(100);
        }

        if (num_times == 0) {
            break;
        }
    }
}

void father() {
    for(;;) {
        for (int i = 1; i <= 12; i++) {
            sem_wait(&goFather);
            printf("Child #%d is read a book by Father\n", i);
        }
        
        printf("\n");

        for (int i = 1; i <= 12; i++) {
            printf("Child #%d is tucked in bed by Father\n", i);
        }

        printf("\n");

        num_times--;
        if (num_times == 0) {
            break;
        }

        sem_post(&goMother);

        // int value;
        // sem_getvalue(&goMother, &value);
        printf("Father is going to sleep and waking up Mother to take care of the children.\n\n");
    }
}


int main(int argc, char *argv[]) {

    //Exit program if invalid number of arguments are given
    if (argc != 2) {
        printf("Program requires 1 argument: number of day/night cycles.\n");
        exit(1);
    }
    num_times = atoi(argv[1]);
    pthread_t p1;
    pthread_t p2;
    sem_init(&goMother, 0, 1);
    sem_init(&goFather, 0, 0);
    
    pthread_create(&p1, NULL, (void*)mother, NULL);
    father();
    

    return 0;
}