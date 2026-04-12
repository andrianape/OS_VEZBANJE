#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <time.h>
#include <sys/types.h>

int main(int argc, char* argv[])
{
    int fd1[2];
    int fd2[2];
    pipe(fd1);
    pipe(fd2);

    if(fork() == 0)
    {
        //proces dete 
        close(fd1[1]);
        close(fd2[0]);
        int N;
        while(read(fd1[0], &N, sizeof(int)) > 0)
        {
            if(N==-1)
                break;

            int niz[10];
            read(fd1[0], niz, N*sizeof(int));
            int suma=0;
            for(int i=0; i<N; i++)
                suma += niz[i];
            double avg = (N > 0) ? (double)suma / N : 0.0;
            write(fd2[1], &suma, sizeof(int));
            write(fd2[1], &avg, sizeof(double));

        }
        close(fd1[0]);
        close(fd2[1]);
    }
    else
    {
        //proces roditelj
        close(fd1[0]);
        close(fd2[1]);
        srand(time(NULL));
        int M = rand() % 5 + 1;
        for(int k = 0; k<M; k++)
        {
            int N = rand() % 11;
            int niz[10];
            for(int i=0; i<N; i++)
            {
                niz[i] = rand() % 101;
            }

            write(fd1[1], &N, sizeof(int));
            write(fd1[1], niz, N*sizeof(int));
            int suma;
            double avg;
            read(fd2[0], &suma, sizeof(int));
            read(fd2[0], &avg, sizeof(double));
            printf("Suma je: %d\n", suma);
            printf("Prosek je: %.2f\n", avg);
        }
        int end = -1;
        write(fd1[1], &end, sizeof(int));
        close(fd1[1]);
        close(fd2[0]);
        wait(NULL);
    }
    return 0;
}