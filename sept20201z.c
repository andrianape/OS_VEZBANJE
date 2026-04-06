#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

int buff[2];

pthread_mutex_t mutex;
pthread_cond_t full;
pthread_cond_t empty;
int isFull = 0;

void* Upis(void* arg)
{
    int j=0;
    for(int i=0; i<20; i++)
    {
        pthread_mutex_lock(&mutex);//zakljucava se mutex
        while(isFull)
        {
            pthread_cond_wait(&empty, &mutex);
            //ceka da dobije signal da od empty da nastavi, mutex se otkljucava
        }
        buff[j] = rand() % 100 + 100;
        j = (j+1) % 2;
        if(j==0)
        {
            isFull = 1;
            pthread_cond_signal(&full);
            //budi se glavna nit
        }
        pthread_mutex_unlock(&mutex);
    }
}

int main(int argc, char* argv[])
{
    pthread_t nit;
    int i;

    pthread_mutex_init(&mutex, NULL);
    pthread_cond_init(&full, NULL);
    pthread_cond_init(&empty, NULL);

    pthread_create(&nit, NULL, (void*)&Upis, NULL);

    for(i=0; i<10; i++)
    {
        pthread_mutex_lock(&mutex);
        while(!isFull)
        {
            pthread_cond_wait(&full, &mutex);
        }
        printf("a = %d, b= %d, s=%d\n", buff[0], buff[1], buff[0]+buff[1]);

        isFull = 0;
        pthread_cond_signal(&empty);
        pthread_mutex_unlock(&mutex);
    }
    return 0;
}