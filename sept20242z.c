#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <fcntl.h>
#define BUF_SIZE 1000

int main(int argc, char* argv[])
{
    if(argc < 3)
    {
        printf("Nedovoljan broj argumenata!\n");
        exit(1);
    }

    int fd[2];
    pipe(fd);
    int fd_in = open(argv[1], O_RDONLY);
    int fd_out = open(argv[2], O_WRONLY | O_CREAT | O_TRUNC | 0644);

    if(fork() == 0)
    {
        //proces dete
        //dobija br bajtova od roditelja koje je procitao iz datoteke
        //zatim kriptujes svaki bajt sa njegovim rednim brojem
        //i upisuje ga u odredisnu datoteku
        //pa nakon toga originalni bajt

        close(fd[1]);
        char buffer[BUF_SIZE];
        int procitaniBajtovi;
        int index = 0; // redni broj bajta

        while(1)
        {
            read(fd[0], &procitaniBajtovi, sizeof(int));
            if(procitaniBajtovi == 0)
                break;

                read(fd[0], buffer, procitaniBajtovi);
                for(int i=0; i< procitaniBajtovi; i++)
                {
                    char kriptovanBajt = buffer[i] ^ index;
                    write(fd_out, &kriptovanBajt, 1);
                    write(fd_out, &buffer[i], 1);
                    index++;
                }
        }
        close(fd[0]);
    }
    else 
    {
        //proces roditelj
        //on cita po 1000 bajtova iz datoteke 
        //salje broj bajtova procitanih
        //pa salje tekst
        close(fd[0]);
        char buffer[BUF_SIZE];
        int procitaniBajtovi;

        while(procitaniBajtovi = read(fd_in, buffer, BUF_SIZE) > 0)
        {
            write(fd[1], &procitaniBajtovi, sizeof(int));
            write(fd[1], buffer, procitaniBajtovi);
        }
        procitaniBajtovi = 0; //izaslo se iz petlje
        write(fd[1], &procitaniBajtovi, sizeof(int));
        close(fd[1]);
        wait(NULL);
    }
    close(fd_in);
    close(fd_out);
    return 0;
}