#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <string.h>

int main(int argc, char* argv[])
{
    FILE* f=fopen(argv[argc-1], "w"); // otvaramo fajl u kome redirektujemo izlaz i menjamo b u B
    //da probam argv[argc-1]
    int fd[2];
    pipe(fd);
    if(argc < 2)
    {
        printf("Nedovoljan broj argumenata\n");
        exit(1);
    }
    if(fork() == 0)
    {
        //proces dete izlaz stdout odg komande prosledjuje u datoteku
        char* buff[16]={0};
        //svi su na NULL jer kod exec-a poslednji argument mora da bude NULL
        close(fd[0]);
        for(int i=1; i<argc-1; i++)
        {
            buff[i-1]=argv[i];//upisuje se komanda
        }
        printf("%s", buff[0]);//komanda koju izvrsavamo
        dup2(fd[1], 1); //ako ne radi STDOUT_FILENO
        if(execvp(buff[0], buff) == -1) //kod ove komande prvi argument je komanda koja se izvrsava ili fajl, a drugi su argumenti te komande
        {
            printf("Greska\n");
            exit(1);
        }
        close(fd[1]);

    }
    else
    {
        wait(NULL);
        close(fd[1]);
        char c;
        while(read(fd[0], &c, 1) > 0)
        {
            if(c == 'b')
                c ='B';
            fputc(c,f);
        }
        close(fd[0]);
    }
    fclose(f);
    return 0;
}