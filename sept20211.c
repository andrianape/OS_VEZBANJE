#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <string.h>

void* broji(void* arg)
{
    int n =*((int*)arg);
    for(int i=0; i<n; i++)
    {
        printf("%d\n", i);
        sleep(3);
    }
}
int main(int argc, char* argv[])
{
    int x;
    char t[5];
    pthread_t nit;
    printf("UNesi broj: ");
    scanf("%s", t);
    while(strcmp(t, "KRAJ") != 0)
    {
        x = atoi(t);
        pthread_create(&nit, NULL, &broji, &x);
        pthread_join(nit, NULL);
        printf("Unesi broj: ");
        scanf("%s", t);
    }
    return 0;
}