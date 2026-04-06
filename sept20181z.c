#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <time.h>
pthread_cond_t full;
pthread_cond_t empty;
pthread_mutex_t mutex;

int isFull = 0;

int buff[2];
void* unesi(void* arg)
{
    int j=0;
    for(int i=0; i<20; i++)
    {
        pthread_mutex_lock(&mutex);
        while(isFull)
        {
            pthread_cond_wait(&empty, &mutex);
        }
        buff[j] = rand() % 100 + 100;
        j = (j+1) % 2;
        if(j==0)
        {
            isFull = 1;
            pthread_cond_signal(&full);
        }
        pthread_mutex_unlock(&mutex);
    }
}

int main(int argc, char* argv[])
{
    srand(time(0));
    pthread_t nit;
    pthread_mutex_init(&mutex, NULL);
    pthread_cond_init(&full, NULL);
    pthread_cond_init(&empty, NULL);

    pthread_create(&nit, NULL, (void*)&unesi, NULL);
    for(int i=0; i<10; i++)
    {
        pthread_mutex_lock(&mutex);
        while(!isFull)
        {
            pthread_cond_wait(&full, &mutex);
        }
        if(buff[0] == buff[1])
            printf("Ovi brojevi su jednaki!\n");
        else
            printf("Ovi brojevi nisu jednaki!\n");
        
        isFull = 0;
        pthread_cond_signal(&empty);
        pthread_mutex_unlock(&mutex);
    }
}