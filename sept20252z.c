#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <sys/types.h>
#include <sys/wait.h>

int main(int argc, char* argv[])
{
    int fd[2];
    if(pipe(fd) < 0)
    {
        printf("Greska prilikom kreiranja pipe\n");
        exit(1);
    }
    if(fork() == 0)
    {
        //proces dete 
        int broj;
        close(fd[1]);
        while(read(fd[0], &broj, sizeof(int)) > 0)
        {
            if(broj % 2 == 0)
            {
                printf("Paran broj: %d\n", broj);
            }
        }
        close(fd[0]);
    }
    else
    {
        //proces roditelj
        close(fd[0]);
        srand(time(0));
        int neparni = 0;
        while(1) // unose se br dok neparni >=3
        {
            int broj = rand() % 100;
            write(fd[1], &broj, sizeof(int));

            if(broj % 2 != 0)
                neparni++;
            if(neparni >= 3)
                break;
        }
        close(fd[1]);
        wait(NULL);
    }
    return 0;
}