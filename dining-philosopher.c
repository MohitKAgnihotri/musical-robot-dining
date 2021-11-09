#include <pthread.h>
#include <stdio.h>
#include <unistd.h>
#include <time.h>
#include <stdlib.h>
#include <semaphore.h>

#define NUM_PHILOSOPHERS 5

void *dining_philosopher_thread_func(void *);
void dining_philosopher_think(int);
void dining_philosopher_take_forks(int);
void dining_philosopher_eat(int);
void dining_philosopher_put_forks(int);

sem_t forks[NUM_PHILOSOPHERS];
pthread_t philosophers[NUM_PHILOSOPHERS];
pthread_attr_t attributes[NUM_PHILOSOPHERS];

int main() {
    int i;
    srand(time(NULL));
    for (i = 0; i < NUM_PHILOSOPHERS; ++i) {
        sem_init(&forks[i],0,1);
    }

    for (i = 0; i < NUM_PHILOSOPHERS; ++i) {
        pthread_attr_init(&attributes[i]);
    }

    for (i = 0; i < NUM_PHILOSOPHERS; ++i) {
        pthread_create(&philosophers[i], &attributes[i], dining_philosopher_thread_func, (void *)(i));
    }

    for (i = 0; i < NUM_PHILOSOPHERS; ++i) {
        pthread_join(philosophers[i], NULL);
    }
    return 0;
}

void *dining_philosopher_thread_func(void *philosopherNumber) {
    int current_philosopherNumber = (int)philosopherNumber;
    while (1) {
        dining_philosopher_think(current_philosopherNumber);
        dining_philosopher_take_forks(current_philosopherNumber);
        dining_philosopher_eat(current_philosopherNumber);
        dining_philosopher_put_forks(current_philosopherNumber);
    }
}

void dining_philosopher_think(int philosopherNumber) {
    int sleepTime = rand() % 3 + 1;
    printf("Philosopher %d will think for %d seconds\n", philosopherNumber, sleepTime);
    sleep(sleepTime);
}

void dining_philosopher_take_forks(int philosopherNumber) {
    int right = (philosopherNumber + 1) % NUM_PHILOSOPHERS;
    int left = (philosopherNumber + NUM_PHILOSOPHERS) % NUM_PHILOSOPHERS;
    if (philosopherNumber & 1) {
        printf("Philosopher %d is waiting to pick up chopstick %d\n", philosopherNumber, right);
        sem_wait(&forks[right]);
        printf("Philosopher %d picked up chopstick %d\n", philosopherNumber, right);
        printf("Philosopher %d is waiting to pick up chopstick %d\n", philosopherNumber, left);
        sem_wait(&forks[left]);
        printf("Philosopher %d picked up chopstick %d\n", philosopherNumber, left);
    }
    else {
        printf("Philosopher %d is waiting to pick up chopstick %d\n", philosopherNumber, left);
        sem_wait(&forks[left]);
        printf("Philosopher %d picked up chopstick %d\n", philosopherNumber, left);
        printf("Philosopher %d is waiting to pick up chopstick %d\n", philosopherNumber, right);
        sem_wait(&forks[right]);
        printf("Philosopher %d picked up chopstick %d\n", philosopherNumber, right);
    }
}

void dining_philosopher_eat(int philosopherNumber) {
    int eatTime = rand() % 3 + 1;
    printf("Philosopher %d will eat for %d seconds\n", philosopherNumber, eatTime);
    sleep(eatTime);
}

void dining_philosopher_put_forks(int philosopherNumber) {
    printf("Philosopher %d will will put down her forks\n", philosopherNumber);
    sem_post(&forks[(philosopherNumber + 1) % NUM_PHILOSOPHERS]);
    sem_post(&forks[(philosopherNumber + NUM_PHILOSOPHERS) % NUM_PHILOSOPHERS]);
}