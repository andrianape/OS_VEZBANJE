#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
int niz[10];
pthread_mutex_t mutex;
pthread_cond_t paran;
int jelparan=0;

void* stampaj(void* arg)
{
    //ova nit stampa elemente niza, ako je njihova suma parna
    pthread_mutex_lock(&mutex);
    while(!jelparan)
    {
        pthread_cond_wait(&paran, &mutex);
    }
    for(int i=0; i<10; i++)
    {
        printf("%d\n", niz[i]);
    }
    printf("\n");
    pthread_mutex_unlock(&mutex);
    return NULL;
}

void* modifikuj(void* arg)
{
    //ostale cetiri niti koje dodaju vr u opsegu -10, 10
    //slucajno izabranom elementu niza
    pthread_mutex_lock(&mutex);
    int p = rand() % 10;//slucajno izabran el niza
    niz[p] = rand() % 21 - 10;
    //vr u opsegu -10 i 10
    int s = 0; // suma svih elemenata
    for(int i=0; i<10; i++)
    {
        s+=niz[i];
    }
    if(s%2 == 0)
    {
        jelparan=1;
        pthread_cond_signal(&paran);
    }
    sleep(3);
    pthread_mutex_unlock(&mutex);
    return NULL;
}

int main(int argc, char* argv[])
{
    pthread_t niti[5];
    pthread_mutex_init(&mutex, NULL);
    pthread_cond_init(&paran, NULL);

    for(int i=0; i<10; i++)
    {
        niz[i] = rand() % 20 +1;
    }
    pthread_create(&niti[4], NULL, stampaj, NULL);
    for(int i=0; i<4; i++)
    {
        pthread_create(&niti[i], NULL, modifikuj, NULL);
    }
    for(int i=0; i<4; i++)
    {
        pthread_join(niti[i], NULL);
    }
    pthread_join(niti[4], NULL);
    return 0;
}
// #include <stdio.h>
// #include <pthread.h>
// #include <unistd.h>
// #include <stdlib.h>
// #include <time.h>

// #define N 10

// int niz[N];
// pthread_mutex_t mutex;
// pthread_cond_t paran;
// int jelparan = 0;

// void *pisi(void *arg) {
//     pthread_mutex_lock(&mutex);
//     while (!jelparan) {
//         pthread_cond_wait(&paran, &mutex);
//     }

//     printf("Niz kada je zbir paran:\n");
//     for (int i = 0; i < N; i++) {
//         printf("%d ", niz[i]);
//     }
//     printf("\n");

//     pthread_mutex_unlock(&mutex);
//     return NULL;  // OBAVEZNO
// }

// void *modifikuj(void *arg) {
//     pthread_mutex_lock(&mutex);

//     int p = rand() % N;          // nasumični indeks
//     int delta = rand() % 21 - 10; // nasumično -10..10
//     niz[p] += delta;

//     int s = 0;
//     for (int i = 0; i < N; i++) {
//         s += niz[i];
//     }

//     if (s % 2 == 0) {
//         jelparan = 1;
//         pthread_cond_signal(&paran); // probudi nit pisi
//     }

//     pthread_mutex_unlock(&mutex);
//     sleep(1); // simulacija rada niti
//     return NULL;  // OBAVEZNO
// }

// int main() {
//     pthread_t niti[5];

//     pthread_mutex_init(&mutex, NULL);
//     pthread_cond_init(&paran, NULL);

//     srand(time(NULL)); // SEME za rand() → obavezno

//     // inicijalizuj niz slučajnim brojevima 1..20
//     printf("Niz na pocetku:\n");
//     for (int i = 0; i < N; i++) {
//         niz[i] = rand() % 20 + 1;
//         printf("%d ", niz[i]);
//     }
//     printf("\n");

//     // kreiraj nit koja ispisuje niz
//     pthread_create(&niti[4], NULL, pisi, NULL);

//     // kreiraj 4 niti koje modifikuju niz
//     for (int i = 0; i < 4; i++) {
//         pthread_create(&niti[i], NULL, modifikuj, NULL);
//     }

//     // čekaj sve niti
//     for (int i = 0; i < 4; i++) {
//         pthread_join(niti[i], NULL);
//     }
//     pthread_join(niti[4], NULL);

//     pthread_mutex_destroy(&mutex);
//     pthread_cond_destroy(&paran);

//     return 0;
// }