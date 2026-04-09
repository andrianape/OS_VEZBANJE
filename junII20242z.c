#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/wait.h>

#define MAX 1024
#define MAX_WORDS 50
#define WORD_LEN 50

int main(int argc, char* argv[])
{
    if(argc < 3)
    {
        printf("Nedovoljan broj argumenata\n");
        exit(1);
    }

    int fd1[2]; //roditelj i dete 1
    int fd2[2];  // dete 1 i dete 2
    pipe(fd1);
    pipe(fd2);

    if(fork() == 0)
    {
        close(fd1[1]);
        close(fd2[0]);

        char filename[100];
        int n; // broj reci
        //citanje imena fajla i broj reci
        read(fd1[0], filename, sizeof(filename));
        read(fd1[0], &n, sizeof(int));

        char reci[MAX_WORDS][WORD_LEN];

        for(int i=0; i<n; i++)
        {
            read(fd1[0], reci[i], WORD_LEN);
        }
        FILE* f = fopen(filename, "r");
        char linija[MAX];
        int brLinije = 1;

        while(fgets(linija, MAX, f))
        {
            for(int i=0; i<n; i++)
            {
                if(strstr(linija, reci[i]) != NULL)
                {
                    write(fd2[1], &brLinije, sizeof(int));
                }
            }
            brLinije++;
        }
        fclose(f);
        close(fd1[0]);
        close(fd2[1]);
    }
    else if(fork() == 0)
    {
        //dete dva
        close(fd1[0]);
        close(fd1[1]);
        close(fd2[1]);
        int suma=0;
        int broj;
        while(read(fd2[0], &broj, sizeof(int)) > 0)
        {
            suma += broj;
        }
        printf("Zbir rednih br. linija je: %d\n", suma);
        close(fd2[0]);
    }
    else
    {
        //roditelj
        close(fd1[0]);
        close(fd2[0]);
        close(fd2[1]);

        //broj reci
        int n = argc - 2;
        //salje fajl, pa broj reci i reci
        write(fd1[1], argv[1], 100);
        write(fd1[1], &n, sizeof(int));

        for(int i=0; i<n; i++)
        {
            write(fd1[1], argv[i+2], WORD_LEN);
        }
        close(fd1[1]);
        wait(NULL);
        wait(NULL);
    }
    return 0;
}