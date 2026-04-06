#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
int main(int argc, char* argv[])
{
    srand(time(0));
    int fd[2];
    if(pipe(fd) < 0)
    {
        printf("Greska prilikom kreiranja pipe-a\n");
        exit(1);
    }
    if(fork() == 0)
    {
        //proces dete cita brojeve i proverava da li su deljivi sa 3
        int x;
        close(fd[1]);
        for(int i=0; i<20; i++)
        {
            read(fd[0], &x, sizeof(int));
            if(x % 3 == 0)
                printf("%d\n", x);
        }
        close(fd[0]);
    }
    else
    {
        //proces roditelj koji generise random br od 100 do 199
        //i salje ih detetu
        close(fd[0]);
        int x;
        for(int i=0; i<20; i++)
        {
            x = rand() % 100 + 100;
            write(fd[1], &x, sizeof(int));
        }
        close(fd[1]);
    }
    return 0;
}