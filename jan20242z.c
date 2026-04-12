#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <sys/wait.h>
#define SIZE 100

int main(int argc, char* argv[])
{
    int fd1[2];
    int fd2[2];
    pipe(fd1);
    pipe(fd2);

    if(fork() == 0)
    {
        //proces dete cita rec
        // ako ima cifre racuna sumu
        // i salje roditelju 
        close(fd1[1]);
        close(fd2[0]);
        char rec[SIZE];

        while(1)
        {
            int r = read(fd1[0], rec, SIZE);
            if(r <= 0)
                break;
            int suma = 0;
            for(int i=0; rec[i] != '\0'; i++)
            {
                if(isdigit(rec[i]))
                {
                    suma +=rec[i] - '0';
                    //ovo oduzimanje '0' se radi da bi se tacno dobio recimo broj 5 je se to salje kao '5' - '0' se dobija broj 5
                }
            }
            write(fd2[1], &suma, sizeof(int));
        }
        close(fd1[0]);
        close(fd2[1]);

    }
    else
    {
        //proces roditelj
        //unosi se recenica, salje rec pa cita sumu pa salje opet rec iz recenice
        close(fd1[0]);
        close(fd2[1]);
        char recenica[1024];

        printf("Unesi recenicu: ");
        fgets(recenica, sizeof(recenica), stdin);
        char* token = strtok(recenica, " ");
        while(token != NULL)
        {
            write(fd1[1], token, SIZE);
            int suma;
            read(fd2[0], &suma, sizeof(int));
            printf("Suma cifara u %s je %d\n", token, suma);
            token = strtok(NULL, " ");
        }
        close(fd1[1]);
        close(fd2[0]);
        wait(NULL);
    }
    return 0;
}