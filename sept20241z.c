#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>

void* broji(void* arg)
{
    int n = *((int*)arg);
    for(int i=0; i<n; i++)
    {
        printf("%d\n", i);
        sleep(2);
    }
}

int main(int argc, char* argv[])
{
    int x; 
    char t[5];
    pthread_t nit;
    printf("Uneti broj: ");
    scanf("%s", t);
    x = atoi(t);
    while(x!= 999)
    {
        pthread_create(&nit, NULL, &broji, &x);
        pthread_join(nit, NULL);
        printf("Uneti broj: ");
        scanf("%s", t);
        x = atoi(t);
    }
    return 0;
}