#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <semaphore.h>
#include <pthread.h>
#define DUZINA 80

//semafori
sem_t mutex;
sem_t proca;
sem_t procb;

//fja za nitA
void* nitA(void* arg)
{
    char* fajl = (char*)arg;
    FILE* fileA = fopen(fajl, "r");
    char linija[DUZINA];
    int i=1;
    while(fgets(linija, DUZINA, fileA) != NULL)
    {
        sem_wait(&proca);
        sem_wait(&mutex);

        FILE* fileC = fopen("zbir.txt", "a");
        if(fileC)
        {
            fprintf(fileC,"prva.txt:%d:%s", i, linija);
            fclose(fileC);
        }
        sem_post(&mutex);
        sem_post(&procb);
        i++;
    }
    fclose(fileA);
    return NULL;
}

//fja za nit b
void* nitB(void* arg)
{
    char* fajlb = (char*)arg;
    FILE* f = fopen(fajlb, "r");
    char linija[DUZINA];
    int i=1;
    while(fgets(linija, DUZINA, f) != NULL)
    {
        sem_wait(&procb);
        sem_wait(&mutex);

        FILE* fileC = fopen("zbir.txt", "a");
        if(fileC)
        {
            fprintf(fileC, "druga.txt:%d:%s", i, linija);
            fclose(fileC);
        }
        sem_post(&mutex);//otkljucaj mutex
        sem_post(&proca); //dozvoli nitA
        i++;
    }
    fclose(f);
    return NULL;
}

int main(int argc, char* argv[])
{
    pthread_t tredA, tredB;
    //inicijalizacija semafora
    sem_init(&mutex, 0, 1);
    sem_init(&proca, 0, 1);
    sem_init(&procb, 0, 0);
    //kreiranje niti
    pthread_create(&tredA,NULL, nitA, "prva.txt");
    pthread_create(&tredB, NULL, nitB, "druga.txt");

    //cekanje da niti zavrse
    pthread_join(tredA, NULL);
    pthread_join(tredB, NULL);

    //unistavanje semafora
    sem_destroy(&mutex);
    sem_destroy(&proca);
    sem_destroy(&procb);
    return 0;
}