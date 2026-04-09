#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <time.h>

int N;
int trenutni=0; //kao brojac

pthread_mutex_t mutex;
pthread_cond_t deljivi;
pthread_cond_t ostali;

void* nit_deljivi(void* arg)
{
    while(1)
    {
        pthread_mutex_lock(&mutex);

        while(trenutni <=N && trenutni % 7 != 0)
        {
             pthread_cond_wait(&deljivi, &mutex);
        }

        if(trenutni > N)
        {
            pthread_mutex_unlock(&mutex);
            break;
        }
        printf("Stampa nit_deljivi: %d\n", trenutni);
        trenutni++;
        pthread_cond_signal(&ostali);
        pthread_mutex_unlock(&mutex);
    }
}

void* nit_ostali(void* arg)
{
    while(1)
    {
        pthread_mutex_lock(&mutex);

        while(trenutni <=N && trenutni % 7 == 0)
        {
            pthread_cond_wait(&ostali, &mutex);
        }

        if(trenutni > N)
        {
            pthread_mutex_unlock(&mutex);
            break;
        }
        printf("Stampa nit_ostali: %d\n", trenutni);
        trenutni++;
        pthread_cond_signal(&deljivi);
        pthread_mutex_unlock(&mutex);
    }
}

int main(int argc, char* argv[])
{
    if(argc < 2)
    {
        printf("Nedovoljan broj argumenata\n");
        exit(1);
    }

    N = atoi(argv[1]);

    pthread_t t1, t2;
    pthread_mutex_init(&mutex, NULL);
    pthread_cond_init(&deljivi, NULL);
    pthread_cond_init(&ostali, NULL);

    pthread_create(&t1, NULL, nit_deljivi, NULL);
    pthread_create(&t1, NULL, nit_ostali, NULL);
    pthread_join(t1, NULL);
    pthread_join(t2, NULL);

    pthread_mutex_destroy(&mutex);
    pthread_cond_destroy(&deljivi);
    pthread_cond_destroy(&ostali);

    return 0;
}