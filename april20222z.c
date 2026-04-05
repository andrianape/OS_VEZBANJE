#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <time.h>

int main(int argc, char* argv[])
{
    int fd1[2]; // posto je dvosmerna kom. treba nam 2 datavoda
    int fd2[2];
    //fd[0] za citanje, fd[1] za upisivanje

    if(pipe(fd1) < 0)
    {
        printf("Greska kod pipe1");
        exit(1);
    }
    if(pipe(fd2) < 0)
    {
        printf("Greska kod pipe2");
        exit(1);
    }
    srand(time(0));//u sustini ovo se koristi
    //da kada pokrenemo program da nam ne generise svaki put
    //iste brojeve, vec razlicite
    if(fork() == 0)
    {
        //proces dete, cita brojeve
        //proverava da li su deljivi sa 3 dodaje im vrednost 25
        //i salje ih roditelju
        int niz[10];
        close(fd1[1]);
        close(fd2[0]);
        read(fd1[0], niz, 10*sizeof(int));
        close(fd1[0]);
        for(int i=0; i<10; i++)
        {
            if(niz[i] % 3 == 0)
            {
                niz[i]+=25;
                write(fd2[1], &niz[i], sizeof(int));
            }
        }
        close(fd2[1]);
    }
    else
    {
        //proces roditelj generise 10 slucajnih brojeva
        //u opsegu od 200 do 299 i salje ih detetu
        //dete se izvrsava salje roditelju odg brojeve i 
        //roditelj ih stampa
        close(fd1[0]);
        close(fd2[1]);
        int niz[10];
        for(int i=0; i<10; i++)
        {
            niz[i] = rand() % 100 + 200;
        }
        write(fd1[1], niz, 10*sizeof(int));
        close(fd1[1]);
        int j=0;
        int i=0;
        int x;
        int niz1[10];
        while(j < 10)
        {
            int eof = read(fd2[0], &x, sizeof(int));
            if(eof != 0)
            {
                niz1[i++] = x;
            }
            j++;
        }
        close(fd2[0]);
        for(int k=0; k<i; k++)
        {
            printf("%d\n", niz1[k]);
        }
    }
    return 0;
}
