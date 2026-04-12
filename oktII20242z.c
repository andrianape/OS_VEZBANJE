#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <string.h>

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

        char datoteka[20];
        char rec[10];
        int brLinije=1;
        char linija[80];

        read(fd1[0], datoteka, sizeof(datoteka));
        read(fd1[0], &rec, sizeof(rec));
        close(fd1[0]);

        FILE* f = fopen(datoteka, "r");
        while(fgets(linija, sizeof(linija), f))
        {
            if(strstr(linija, rec) != NULL)
            {
                write(fd2[1], &brLinije, sizeof(int));
            }
            brLinije++;
        }
        fclose(f);
        close(fd2[1]);
    }
    else
    {
        //proces roditelj
        close(fd1[0]);
        close(fd2[1]);

        char putanja[20];
        char rec[10];
        printf("Uneti datoteku: ");
        fgets(putanja, 20, stdin);
        putanja[strcspn(putanja, "\n")]=0;
        printf("Uneti kljucnu rec: ");
        fgets(rec, 10, stdin);
        rec[strcspn(rec, "\n")] = 0;
        write(fd1[1], putanja, 20);
        write(fd1[1], rec, 10);
        close(fd1[1]);

        //prijem rezultata
        int line_num;
        printf("Kljucna rec se pojavljuje u linijama:\n");
        while(read(fd2[0], &line_num, sizeof(int)) > 0)
        {
            printf("%d\n", line_num);
        }
        close(fd2[0]);
    }
    return 0;
}