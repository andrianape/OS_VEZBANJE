#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <string.h>

#define MAX_LINE 1024
#define MAX_NUMS 100

int* niz;
int duzina;
long suma;

void* sumaNiza(void* arg)
{
    suma = 0;
    for(int i=0; i<duzina; i++)
    {
        suma += niz[i];
    }
    return NULL;
}

int main(int argc, char* argv[])
{
    FILE* fajl = fopen("jan20241.txt", "r");
    char linija[MAX_LINE];

    while(fgets(linija, sizeof(linija), fajl))
    {
        niz = malloc(MAX_NUMS * sizeof(int));
        //parsiranje linije u niz 
        int brojac = 0;
        char* token = strtok(linija, " ");
        while(token != NULL)
        {
            niz[brojac++] = atoi(token);
            token = strtok(NULL, " ");
        }
        duzina = brojac;
        pthread_t nit;
        pthread_create(&nit, NULL, sumaNiza, NULL);
        pthread_join(nit, NULL);
        printf("Suma reda: %ld\n", suma);
        free(niz);
    }
    fclose(fajl);
    return 0;
}