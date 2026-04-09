#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>

int main(int argc, char* argv[])
{
    if(argc < 3)
    {
        printf("Nedovoljan broj argumenata\n");
        exit(1);
    }
    if(fork() == 0)
    {
        //proces dete 
        //koristi se execlp, p zato sto nem ora da se zna path recimo komanda ls
        execlp(argv[1], argv[1], argv[2], NULL);

    }
    else 
    {
        int status;
        wait(&status);
        printf("Proces dete zavrsilo sa statusom: %d\n", status);

    }
    return 0;
}