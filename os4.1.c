#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>

#define N 5  
#define NUM_ITEMS 20  

int buf[N];  
int buf_index = 0;    
int items_produced   = 0;
int items_consumed = 0;  

sem_t empty;        
sem_t full;        
pthread_mutex_t mutex;

void* producer(void* arg) {
    int item;
   
    while (1)
    {
        item = rand() % 100;  
       
        sem_wait(&empty);  
       
        pthread_mutex_lock(&mutex);  
       
        buf[buf_index] = item;  
        printf("Produced item: %d\n", item);
        buf_index = (buf_index + 1) % N;  
       
        items_produced++;
       
        pthread_mutex_unlock(&mutex);  
       
        sem_post(&full);  
       
        if (items_produced >= NUM_ITEMS)
            break;
    }
   
    return NULL;
}

void* consumer(void* arg) {
    int item;
   
    while (1) {
        sem_wait(&full);  
       
        pthread_mutex_lock(&mutex);
       
        item = buf[buf_index];  
        printf("Consumed item: %d\n", item);
        buf_index = (buf_index + 1) % N;
       
        items_consumed++;
       
        pthread_mutex_unlock(&mutex);
       
        sem_post(&empty);  
       
        if (items_consumed >= NUM_ITEMS)
            break;  
    }
   
    return NULL;
}

int main()
{
   
    sem_init(&empty, 0, N);
    sem_init(&full, 0, 0);
   
   
    pthread_mutex_init(&mutex, NULL);
   
   
    pthread_t producer_thread, consumer_thread;
    pthread_create(&producer_thread, NULL, producer, NULL);
    pthread_create(&consumer_thread, NULL, consumer, NULL);
   
   
    pthread_join(producer_thread, NULL);
    pthread_join(consumer_thread, NULL);
   
   
    sem_destroy(&empty);
    sem_destroy(&full);
    pthread_mutex_destroy(&mutex);
   
    return 0;
}