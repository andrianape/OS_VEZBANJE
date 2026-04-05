#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <semaphore.h>
#include <pthread.h>

sem_t glavni;
sem_t func;

int broj;

//kod niti fja mora da ima ulazni argument void*
//fja mora da vraca void*
//dakle imamo dva semafora
//func koja je za drugu nit, glavni koji je za glavnu nit
//glavna nit je main, a druga nit je fja broji
//sem_post znaci da se izvrsava nit
//sem_wait ceka odgovarajuca nit da se izvrsi nit koja radi
//u mainu ovde kreiramo samo jednu nit a to je druga nit
//zato sto kada pokrenemo main automatski se pokrece jedna nit
//koriste se DVA SEMAFORA jer je komunikacija obostrana
//kada glavna nit unese broj obavestava drugu nit koja 
//treba da ispisuje brojeve tj stampa do tog broja
//kada zavrsi obavestava glavnu nit da unese sledeci broj
void* broji(void *arg)
{
    while(1)
    {
        sem_wait(&func); // ceka od glavnu nit dozvolu
        if(broj == 99)
            break;
        for(int i=0; i<broj; i++)
        {
            printf("%d\n", i);
            sleep(3);
        }
        sem_post(&glavni);//kada zavrsi daje dozvolu glavnoj niti da nastavi
    }
}

int main(int argc, char* argv[])
{
    pthread_t nit;
    sem_init(&func, 0, 0);
    sem_init(&glavni, 0, 0);
    pthread_create(&nit, NULL, &broji, NULL);

    printf("Unesi broj: ");
    scanf("%d", &broj);
    while(broj!=99)
    {
        sem_post(&func);//daje dozvolu drugoj niti
        sem_wait(&glavni);//ona se blokira, ali kada druga nit zavrsi dobija dozvolu

        printf("Unesi broj: ");
        scanf("%d", &broj);
    }
    sem_post(&func);

    pthread_join(nit, NULL);
    sem_destroy(&glavni);
    sem_destroy(&func);
    return 0;
}