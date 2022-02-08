#include <stdio.h>
#include <stdlib.h>
#include <semaphore.h>
#include <pthread.h>
#include <unistd.h>

#define BufferSize 5
#define MAX 5

sem_t empty;
sem_t full;
pthread_mutex_t mutex;
int buffer[BufferSize];
int count = 0;

void producer(void *pno) {
    //printf("Test\n");
    int item;
    
    for(int i = 0; i < MAX; i++) {
        
        item = rand();
        sem_wait(&empty);
        pthread_mutex_lock(&mutex);
        buffer[++count] = item;
        
        printf("Producer inserted item %d at Buffer Location %d\n", buffer[count], count);

        pthread_mutex_unlock(&mutex);
        sleep(2);
        sem_post(&full);
    }
}

void consumer(void *cno) {
    
    for ( int i = 0; i < MAX; i++) {
        sem_wait(&full);
        pthread_mutex_lock(&mutex);
        int item = buffer[count];
        
        printf("Consumer consumed item %d from Buffer Location %d\n", item, count);
        count--;

        pthread_mutex_unlock(&mutex);
        sleep(2);
        sem_post(&empty);
    }
}

int main() {
    pthread_t prod, con;
    pthread_mutex_init(&mutex, NULL);
    sem_init(&empty,0, BufferSize);
    sem_init(&full, 0, 0);

    pthread_create(&prod, NULL, (void *)producer, NULL);
    pthread_create(&con, NULL, (void *)consumer, NULL);

    pthread_join(con, NULL);
    pthread_join(prod, NULL);

    pthread_mutex_destroy(&mutex);
    sem_destroy(&empty);
    sem_destroy(&full);

    return 0;
}