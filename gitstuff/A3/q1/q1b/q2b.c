#include <pthread.h>
#include <semaphore.h>
#include <stdio.h>
#include<unistd.h>

sem_t forks[5]; // semaphores for forks
sem_t mutex; // for CS
sem_t bowls; // for keeping track of bowls
pthread_t thread_id[5]; // threads for philosophers
int phil[5] = {0,1,2,3,4}; // fork numbers

void* philosopher(void* arg)
{
    while(1)
    {
        sem_wait(&mutex);
        sem_wait(&bowls);
        int* fork = arg;
        int left = *fork;
        int right = (*fork+1)%5;
        printf("Philosopher %d is thinking\n",*fork+1); 
        sem_wait(&forks[left]);
        sem_wait(&forks[right]);
        printf("Philosopher %d is eating\n",*fork+1);
        sleep(1);
        sem_post(&forks[left]);
        sem_post(&forks[right]);
        sem_post(&bowls);
        sem_post(&mutex);
        sleep(0); // to avoid starvation
    }
}



int main(){
    int i;
    sem_init(&mutex,0,4); // initialising
    sem_init(&bowls,0,2); // initialising

    for(i=0;i<5;i++) // initialize semaphores for forks
    {
        sem_init(&forks[i],0,1);
    }

    for(i=0;i<5;i++) // create threads for philosophers
    {
        // printf("creating philosopher %d\n",i+1);
        pthread_create(&thread_id[i],NULL,philosopher,(void *)&phil[i]);
    }

    for(i=0;i<5;i++) // join threads
    {
        pthread_join(thread_id[i],NULL);
    }

    return 0;
}
