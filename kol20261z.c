//ovaj zadatak ne garantuje da ce svi podaci biti rasporedjeni lepo 
// recimmo unosi se 5 6 3 6 7 
// to ce biti 5 6 6 7 a 3 je blokirana nit 
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <limits.h>
//to je int min
#define N 10

pthread_mutex_t mutex;
pthread_cond_t uslov;
int trenutno = INT_MIN;

void* stampaj(void* arg)
{
    int broj = *((int*)arg);
    
    pthread_mutex_lock(&mutex);

    while(broj < trenutno)
    {
        pthread_cond_wait(&uslov, &mutex);
    }

    printf("%d\n", broj);
    trenutno = broj;
    pthread_cond_broadcast(&uslov);
    pthread_mutex_unlock(&mutex);
}

int main(int argc, char* argv[])
{
    pthread_t niti[N];
    pthread_mutex_init(&mutex, NULL);
    pthread_cond_init(&uslov, NULL);


    printf("Unesi 10 brojeva:\n");

    for(int i=0; i<N; i++)
    {
        int* broj = malloc(sizeof(int));
        scanf("%d", broj);
        pthread_create(&niti[i], NULL, stampaj, broj);

    }
    for(int i=0; i<N; i++)
    {
        pthread_join(niti[i], NULL);
    }

    pthread_mutex_destroy(&mutex);
    pthread_cond_destroy(&uslov);
    return 0;
}