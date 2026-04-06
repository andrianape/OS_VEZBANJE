#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>

int main(int argc, char* argv[])
{
    int fd[2];
    if(pipe(fd) < 0)
    {
        printf("Greska kod kreiranja pipe\n");
        exit(1);
    }

    if(fork() == 0)
    {
        close(fd[1]);
        char path[256];
        char rec[256];

        //citanje iz pipe
        read(fd[0], path, sizeof(path));
        read(fd[0], rec, sizeof(rec));
        close(fd[0]);

        FILE* f = fopen(path, "r");
        if(f == NULL)
        {
            printf("Greska otvaranja datoteke\n");
            exit(1);
        }
        char linija[1024];
        int brojLinije = 1;

        while(fgets(linija, sizeof(linija),f))
        {
            char *token = strtok(linija, " ,\n\t");

            while(token!=NULL)
            {
                if(strcmp(token, rec) == 0)
                {
                    printf("Kljucna rec nadjena u liniji: %d\n", brojLinije);
                    break;
                }
                token = strtok(NULL, " ,\n\t");
            }
            brojLinije++;
        }
        fclose(f);
    }
    else
    {
        //proces roditelj
        close(fd[0]);
        char path[256];
        char rec[256];

        printf("Unesi putanju do fajla: ");
        scanf("%255s", path);
        printf("Unesi kljucnu res: ");
        scanf("%255s", rec);

        //slanje kroz pipe
        write(fd[1], path, sizeof(path));
        write(fd[1], rec, sizeof(rec));
        close(fd[1]);
        wait(NULL);
    }
    return 0;
}