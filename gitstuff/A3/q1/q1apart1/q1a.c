#include <pthread.h>
#include <semaphore.h>
#include <stdio.h>
#include<unistd.h>

sem_t forks[5]; // semaphores for forks
pthread_t thread_id[5]; // threads for philosophers
int phil[5] = {0,1,2,3,4}; // fork numbers

void* philosopher(void* arg)
{
    while(1)
    {
        int* fork = arg;
        int left = *fork;
        int right = (*fork+1)%5;
        // printf("Philosopher %d is thinking\n",*fork+1);
        sem_wait(&forks[left]);
        sem_wait(&forks[right]);
        printf("Philosopher %d is eating\n",*fork+1);
        sem_post(&forks[left]);
        sem_post(&forks[right]);
        sleep(0); // to avoid starvation
    }
}

void* ulta(void* arg) // to avoid deadlock
{
    while (1)
    {
        int* fork = arg;
        int left = 4;
        int right = 0;
        // printf("Philosopher %d is thinking\n",*fork+1);
        sem_wait(&forks[left]);
        sem_wait(&forks[right]);
        printf("Philosopher %d is eating\n",*fork+1);
        sem_post(&forks[left]);
        sem_post(&forks[right]);
        sleep(0); // to avoid starvation
    }
}

int main(){
    int i;

    for(i=0;i<5;i++) // initialize semaphores for forks
    {
        sem_init(&forks[i],0,1);
    }

    for(i=0;i<4;i++) // create threads for philosophers
    {
        // printf("creating philosopher %d\n",i+1);
        pthread_create(&thread_id[i],NULL,philosopher,(void *)&phil[i]);
    }

    pthread_create(&thread_id[4],NULL,ulta,(void *)&phil[i]);

    for(i=0;i<5;i++) // join threads
    {
        pthread_join(thread_id[i],NULL);
    }

    return 0;
}
