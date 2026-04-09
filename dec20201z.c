#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>

int main(int argc, char* argv[])
{
    srand(time(0));
    int fd1[2];
    int fd2[2];
    pipe(fd1);
    pipe(fd2);
    if(fork()== 0)
    {
        int p= fork();
        if(p == 0)
        {
            //drugi proces
            close(fd1[1]);
            close(fd2[0]);
            close(fd2[1]);
            char niz[20];
            read(fd1[0], niz, 20);
            close(fd1[0]);
            FILE* f = fopen("KARAKTERI.txt", "w");
            //niz[20]='\0';
            fprintf(f, "%s", niz);
             //fwrite(niz, 1, n, f);
            fclose(f);
        }
        else
        {
            //treci proces
            close(fd1[0]);
            close(fd1[1]);
            close(fd2[1]);
            char niz[20];
            read(fd2[0], niz, 20);
            close(fd2[0]);
            for(int i=0; i<20; i++)
            {
                printf("%c", niz[i]);
            }
        }
    }
    else
    {
        //proces roditelj, prvi proces
        close(fd1[0]);
        close(fd2[0]);
        char niz[20];
        int s=0;
        for(int i=0; i<20; i++)
        {
            niz[i] = (unsigned char)(rand() % 95 +32);
            s+=niz[i];
        }
        if(s % 2 == 0)
        {
            write(fd1[1], niz, 20);
        }
        else
        {
            write(fd2[1], niz, 20);
        }
        close(fd1[1]);
        close(fd2[1]);

    }
    return 0;

}