#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include <time.h>

#define N 4
int buff[4];
int spreman = 0; //da li ima novih pod 
pthread_mutex_t mutex;
pthread_cond_t uslov;

//nit koja generise brojeve
void* generisi(void* arg)
{
    while(1)
    {
        pthread_mutex_lock(&mutex);
        while(spreman == 1)
        {
            pthread_cond_wait(&uslov, &mutex);
        }
        printf("Generisani brojevi: ");
        for(int i=0; i<N; i++)
        {
            buff[i] = rand() % 100;
            printf("%d ", buff[i]);
        }
        printf("\n");
        spreman = 1;
        pthread_cond_signal(&uslov);
        pthread_mutex_unlock(&mutex);
    }
}

int main(int argc, char* argv[])
{
    srand(time(0));
    pthread_t nit;
    pthread_mutex_init(&mutex, NULL);
    pthread_cond_init(&uslov, NULL);
    pthread_create(&nit, NULL,(void*)&generisi, NULL);

    while(1)
    {
        pthread_mutex_lock(&mutex);
        //cekamo da spreman=1
        while(spreman == 0)
        {
            pthread_cond_wait(&uslov, &mutex);
        }

        int suma = 0;
        for(int i=0; i<N; i++)
        {
            suma += buff[i];
        }
        printf("Suma je:%d ", suma);
        if(suma < 50)
            printf("Suma manja od 50\n");
        else if(suma == 50)
            printf("suma = 50\n");
        else    
            printf("Suma veca od 50\n");

        //prekid uslova
        if(suma > 250)
        {
            printf("SUma veca od 150 --> KRAJ\n");
            pthread_mutex_unlock(&mutex);
            break;
        }
        spreman = 0;
        pthread_cond_signal(&uslov);
        pthread_mutex_unlock(&mutex);
    }
    pthread_join(nit, NULL);
    pthread_mutex_destroy(&mutex);
    pthread_cond_destroy(&uslov);
    return 0;
}