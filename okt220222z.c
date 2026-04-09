#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#define BUF_SIZE 256
int main(int argc, char* argv[])
{
    int fd[2];
    pipe(fd);

    if(fork() == 0)
    {
        //proces dete
        char rec[BUF_SIZE];
        close(fd[1]);
        int n;
        while((n=read(fd[0], rec, BUF_SIZE-1)) > 0)
        {
            rec[n] = '\0'; 
            printf("Procitao : %s\n", rec);
        }
        close(fd[0]);
        exit(0);
    }
    else
    {
        //proces roditelj
        close(fd[0]);
        char buff[BUF_SIZE] ="";

        for(int i=argc-1; i>0; i--)
        {
            strcat(buff, argv[i]);
            strcat(buff, " ");
        }
        //posalji string u pipe
        write(fd[1], buff, strlen(buff));
        close(fd[1]);
        execl(argv[0], argv[0], buff, NULL);
        wait(NULL);
    }
    return 0;
}