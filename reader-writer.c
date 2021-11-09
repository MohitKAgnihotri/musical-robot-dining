#include <pthread.h>
#include <semaphore.h>
#include <stdio.h>
#include<unistd.h>
#include<stdlib.h>
#include<limits.h>

sem_t wrt;
pthread_mutex_t mutex;
int cnt = 1;
int readCount = 0;


/* Function Signatures */
void reader_writer_perform_read(void *reader_number);
void reader_writer_perform_write(void *writer_number);
void pthread_reader_function(void *reader_number);
void pthread_writer_function(void *writer_number);

/* Perform Read operation on the count variable.*/
void reader_writer_perform_read(void *rno) {
    // Reading Section
    printf("Reader %d: read cnt as %d\n", *((int *)rno), cnt);
}

void reader_writer_perform_write(void *wno) {
    // since we are running it inside the infinite loop , the value of cnt may go beyond the limit of 32 bit int
    if ( ((long long int)cnt * 2) > INT_MAX)
        cnt = 1;
    else
        cnt += 1;
    printf("Writer %d: modified cnt to %d\n", (*((int *)wno)), cnt);
}

void pthread_writer_function(void *wno)
{
    while (1) {

        // writer's job is to simply write and exit, so it interacts with wrt semaphor only
        sem_wait(&wrt);

        reader_writer_perform_write(wno);

        sem_post(&wrt);

        // sleep is used just to slow down the output on the terminal
        sleep(2);
    }
}

void pthread_reader_function(void *rno)
{
    while (1) {

        // Reader acquire the lock before modifying readCount
        pthread_mutex_lock(&mutex);
        readCount++;
        if (readCount == 1) {
            sem_wait(&wrt); // If this id the first pthread_reader_function, then it will block the writer
        }
        pthread_mutex_unlock(&mutex);

        reader_writer_perform_read(rno);

        // Reader acquire the lock before modifying readCount
        pthread_mutex_lock(&mutex);
        readCount--;
        if (readCount == 0) {
            sem_post(&wrt); // If this is the last pthread_reader_function, it will wake up the writer.
        }
        pthread_mutex_unlock(&mutex);

        // sleep is used just to slow down the output on the terminal
        sleep(2);

    }
}

int main()
{

    pthread_t read[10], write[5];
    pthread_mutex_init(&mutex, NULL);
    sem_init(&wrt, 0, 1);

    int a[10] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10}; //Just used for numbering the producer and consumer

    for (int i = 0; i < 10; i++) {
        pthread_create(&read[i], NULL, (void*)pthread_reader_function, (void*)&a[i]);
    }
    for (int i = 0; i < 5; i++) {
        pthread_create(&write[i], NULL, (void *)pthread_writer_function, (void *)&a[i]);
    }

    for (int i = 0; i < 10; i++) {
        pthread_join(read[i], NULL);
    }
    for (int i = 0; i < 5; i++) {
        pthread_join(write[i], NULL);
    }

    pthread_mutex_destroy(&mutex);
    sem_destroy(&wrt);

    return EXIT_SUCCESS;

}