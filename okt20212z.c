#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/file.h>

int main(int argc, char* argv[])
{
    int fd[2];
    int pid;
    char* args[10];
    int i;

    if(argc < 3)
    {
        printf("Nedovoljan broj argumenata\n");
        exit(1);
    }

    int n = atoi(argv[argc-1]); //zadnji arg je broj linija

    if(pipe(fd) < 0)
    {
        printf("Greska prilikom kreiranja pipe!\n");
        exit(1);
    }

    if(fork() == 0)
    {
        //proces dete
        close(1); // std out zatvoren
        dup(fd[1]);//stdout preusmeren na pipe za upisivanje
        //za execvp moramo napraviti vektor args
        // pri cemu je poslednji argument dakle pre broj linija
        //jednak NULL to je pravilo 

        for(int i=1; i<argc-1; i++)
            args[i-1] = argv[i];

        args[argc-2] = NULL;
        execvp(args[0], args);//mozda if da proverimo da li radi

    }
    else
    {
        //proces roditelj
        close(fd[1]);
        FILE* fp = fdopen(fd[0], "r");
        //koristi se fd jer je pipe file descriptor
        char buffer[1024];
        int count = 0;
        while(fgets(buffer, sizeof(buffer), fp) != 0)
        {
            printf("%s", buffer);
            count++;

            if(count == n)
            {
                printf("-- MORE --");
                getchar();
                count = 0;
            }
        }
        fclose(fp);
    }
    return 0;
}