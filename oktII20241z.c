#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <semaphore.h>
#include <pthread.h>
#define DUZINA 80

sem_t mutex;
sem_t procA;
sem_t procB;
int i=1;
void* nitA(void* arg)
{
    char* fajl = (char*)arg;
    FILE* fA = fopen(fajl, "r");
    char linija[DUZINA];
    //int i=0;
    while(fgets(linija, DUZINA, fA) != NULL)
    {
        sem_wait(&procA);
        sem_wait(&mutex);

        FILE* fileC = fopen("zbirII.txt", "a");
        if(fileC)
        {
            fprintf(fileC, "Prva.txt: %d:%s", i, linija);
            fclose(fileC);
        }
        sem_post(&mutex);
        sem_post(&procB);
        i++;
    }
    fclose(fA);
}

void* nitB(void* arg)
{
    char* fajlb = (char*)arg;
    FILE* fB = fopen(fajlb, "r");
    char linija[DUZINA];
    //int i=1;
    while(fgets(linija, DUZINA, fB) != NULL)
    {
        sem_wait(&procB);
        sem_wait(&mutex);

        FILE* fileC = fopen("zbirII.txt", "a");
        if(fileC)
        {
            fprintf(fileC, "Druga.txt: %d:%s", i, linija);
            fclose(fileC);
        }
        sem_post(&mutex);
        sem_post(&procA);
        i++;
    }
    fclose(fB);
}

int main(int argc, char* argv[])
{
    pthread_t tredA, tredB;
    sem_init(&mutex, 0, 1);
    sem_init(&procA, 0, 1);
    sem_init(&procB, 0, 0);

    pthread_create(&tredA, NULL, nitA, argv[1]);
    pthread_create(&tredB, NULL, nitB, argv[2]);

    pthread_join(tredA, NULL);
    pthread_join(tredB, NULL);

    sem_destroy(&mutex);
    sem_destroy(&procA);
    sem_destroy(&procB);
    return 0;
}