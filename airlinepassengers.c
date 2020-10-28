#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>
#include <time.h>
#include <unistd.h>
#include <string.h>


sem_t lineSem;

sem_t passengerSem;
sem_t baggageSem;
sem_t screenerSem;
sem_t attendantSem;


pthread_mutex_t modifyBaggageArr;
pthread_mutex_t modifyScreenerArr;
pthread_mutex_t modifyAttendantArr;


int passengerIndex = 0;
int baggageIndex = 0;
int screenerIndex = 0;
int attendantIndex = 0;

int * passengerArr;
int * baggageArr;
int * screenerArr;

int num_passengers;
int num_handlers;
int num_screeners;
int num_attendants;


void passenger(void *number) {

    int passengerNo = *((int *) number);
    printf("Passenger #%d arrived at the terminal.\n", passengerNo);
    
    sem_wait(&lineSem);

    sem_wait(&passengerSem);
    passengerArr[passengerIndex] = passengerNo;
    passengerIndex++;
    sem_post(&passengerSem);

    usleep(100);

    printf("Passenger #%d is waiting at baggage processing for a handler.\n", passengerNo);

    sem_post(&baggageSem);
}

void baggageHandler() {

    while(baggageIndex < num_passengers) {

        sem_wait(&baggageSem);

        if (baggageIndex >= num_passengers) {
            break;
        }

        pthread_mutex_lock(&modifyBaggageArr);

        int passengerNo = passengerArr[baggageIndex];
        baggageArr[baggageIndex] = passengerNo;
        baggageIndex++;

        pthread_mutex_unlock(&modifyBaggageArr);

        usleep(100);

        printf("Passenger #%d is waiting to be screened by a screener.\n", passengerNo);

        if (baggageIndex >= num_passengers) {
            for (int i = 0; i < num_handlers; i++) {
                sem_post(&baggageSem);
            }
        }

        sem_post(&screenerSem);

    }
    
}

void securityScreener() {

    while(screenerIndex < num_passengers) {
        
        sem_wait(&screenerSem);

        if (screenerIndex >= num_passengers) {
            break;
        }

        pthread_mutex_lock(&modifyScreenerArr);

        int passengerNo = baggageArr[screenerIndex];
        screenerArr[screenerIndex] = passengerNo;
        screenerIndex++;

        pthread_mutex_unlock(&modifyScreenerArr);

        usleep(100);

        printf("Passenger #%d is waiting to board the plane by an attendant.\n", passengerNo);

        if (screenerIndex >= num_passengers) {
            for (int i = 0; i < num_screeners; i++) {
                sem_post(&screenerSem);
            }
        }

        sem_post(&attendantSem);

    }

}

void flightAttendant() {

    while(attendantIndex < num_passengers) {

        sem_wait(&attendantSem);

        if (attendantIndex >= num_passengers) {
            break;
        }

        pthread_mutex_lock(&modifyAttendantArr);

        int passengerNo = screenerArr[attendantIndex];
        attendantIndex++;

        pthread_mutex_unlock(&modifyAttendantArr);

        usleep(100);

        printf("Passenger #%d has been seated and relaxes.\n", passengerNo);

        if (attendantIndex >= num_passengers) {
            for (int i = 0; i < num_attendants; i++) {
                sem_post(&attendantSem);
            }
        }

    }

}

int main(int argc, char *argv[]) {

    //Exit program if invalid number of arguments are given
    if (argc != 5) {
        printf("Program requires 4 argument: (1) number of passengers, (2) number of baggage handlers (3) number of security screeners (4) number of flight attendants.\n");
        exit(1);
    }
    num_passengers = atoi(argv[1]);
    num_handlers = atoi(argv[2]);
    num_screeners = atoi(argv[3]);
    num_attendants = atoi(argv[4]);

    pthread_t handlers[num_handlers];
    pthread_t screeners[num_screeners];
    pthread_t attendants[num_attendants];
    pthread_t passengers[num_passengers];

    passengerArr = malloc(sizeof(int) * num_passengers);
    memset(passengerArr, 0, sizeof(int));

    baggageArr = malloc(sizeof(int) * num_passengers);
    memset(baggageArr, 0, sizeof(int));  

    screenerArr = malloc(sizeof(int) * num_passengers);
    memset(screenerArr, 0, sizeof(int));

    //create all semaphores

    sem_init(&lineSem, 0, 0);
    sem_init(&passengerSem, 0, 1);
    sem_init(&baggageSem, 0, 0);
    sem_init(&screenerSem, 0, 0);
    sem_init(&attendantSem, 0, 0);

    //create all mutexes
    
    pthread_mutex_init(&modifyBaggageArr, NULL);
    pthread_mutex_init(&modifyScreenerArr, NULL);
    pthread_mutex_init(&modifyAttendantArr, NULL);


    //create all the threads

    for (int i = 0; i < num_handlers; i++) {
        pthread_create(&handlers[i], NULL, (void*)baggageHandler, NULL);
    }

    for (int i = 0; i < num_screeners; i++) {
        pthread_create(&screeners[i], NULL, (void*)securityScreener, NULL);
    }

    for (int i = 0; i < num_attendants; i++) {
        pthread_create(&attendants[i], NULL, (void*)flightAttendant, NULL);
    }

    for (int i = 0; i < num_passengers; i++) {
        int *arg = malloc(sizeof(*arg));
        *arg = i + 1;
        pthread_create(&passengers[i], NULL, (void*)passenger, arg);
    }


    for (int i = 0; i < num_passengers; i++){
        sem_post(&lineSem);
    }
    

    //join all the threads

    for (int i = 0; i < num_handlers; i++) {
        pthread_join(handlers[i], NULL);
    }

    for (int i = 0; i < num_screeners; i++) {
        pthread_join(screeners[i], NULL);
    }

    for (int i = 0; i < num_attendants; i++) {
        pthread_join(attendants[i], NULL);
    }

    for (int i = 0; i < num_passengers; i++) {
        pthread_join(passengers[i], NULL);
    }

    pthread_mutex_destroy(&modifyBaggageArr);
    pthread_mutex_destroy(&modifyScreenerArr);
    pthread_mutex_destroy(&modifyAttendantArr);

    printf("The flight has been boarded and is ready to take off.\n");

    return 0;
}