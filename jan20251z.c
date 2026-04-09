#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>

void* broji(void*arg)
{
    int n=*((int*)arg);
    for(int i=n; i>0; i--)
    {
        printf("%d\n", i);
        sleep(4);
    }
}

int main(int argc, char* argv[])
{
    pthread_t nit;
    int broj;
    printf("Unesi broj: ");
    scanf("%d", &broj);
    while(broj != 0)
    {
        pthread_create(&nit, NULL, &broji, &broj);
        pthread_join(nit, NULL);
        printf("Unesi broj: ");
        scanf("%d", &broj);
    }
    return 0;
}