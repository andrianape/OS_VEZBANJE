#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <pthread.h>

//nit za stampanje
void* stampaj(void* arg)
{
    char* recenica = (char*)arg;
    recenica[0] = toupper(recenica[0]);

    if(recenica[strlen(recenica)-1] != '.')
        strcat(recenica, ".");

    printf("%s\n", recenica);
    return NULL;    
}

//nit za sortiranje
void* sortiraj(void* arg)
{
    char* recenica = (char*)arg;

    char* reci;
    char* nizreci[10];
    char temp[20];

    int i=0;

    reci = strtok(recenica, " ,");
    while(reci != NULL)
    {
        nizreci[i++] = reci;
        reci = strtok(NULL, " ,");
    }

    //sortiranje
    for(int j=0; j<i-1; j++)
    {
        for(int k=j+1; k<i; k++)
        {
            if(strcmp(nizreci[j], nizreci[k]) > 0)
            {
                char *temp = nizreci[j];
                nizreci[j] = nizreci[k];
                nizreci[k] = temp;
            }
        }
    }
    //sastavljanje nazad u recenicu
    char nova[100]="";
    for(int j=0; j<i; j++)
    {
        strcat(nova, nizreci[j]);
        strcat(nova, " ");
    }
    strcpy(recenica, nova);
    return NULL;
}

int main(int argc, char* argv[])
{
    char recenica[100];
    printf("UNesi recenicu: ");
    fgets(recenica, sizeof(recenica), stdin);

    recenica[strcspn(recenica, "\n")] = 0;

    pthread_t nit1, nit2;

    pthread_create(&nit1, NULL, sortiraj, recenica);
    pthread_join(nit1, NULL);

    pthread_create(&nit2, NULL, stampaj, recenica);
    pthread_join(nit2, NULL);

    printf("KRAJ\n");
    return 0;
}