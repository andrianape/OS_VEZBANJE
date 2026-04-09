#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <string.h>

void* Broji(void* arg)
{
    int n = *((int *)arg);
    for(int i=n; i>0; i--)
    {
        printf("%d\n", i);
        sleep(1);
    }
}

int main(int argc, char* argv[])
{
    char t[10];
    pthread_t nit;

    printf("Unesi broj: ");
    scanf("%s", t);
    while(strcmp(t, "KRAJ") != 0)
    {
        int x = atoi(t);
        pthread_create(&nit, NULL, &Broji, &x);
        pthread_join(nit, NULL);

        printf("Unesi broj: ");
        scanf("%s", t);
    }
    return 0;
}