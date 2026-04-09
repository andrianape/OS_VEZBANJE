#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

int main(int argc, char* argv[])
{
    printf("Pokrenut program sa argumentima:\n");
    for(int i = 0; i < argc; i++) {
        printf("argv[%d] = %s\n", i, argv[i]);
    }
    printf("\n");

    if(fork() == 0)
    {
        //proces dete
        switch(argc)
        {
            case 1:
                execl(argv[0], argv[0], NULL);
                break;
            case 2:
                execl(argv[0], argv[1], argv[0], NULL);
                break;
            case 3:
                execl(argv[0], argv[2], argv[1], argv[0], NULL);
                break;
            case 4:
                execl(argv[0], argv[3], argv[2], argv[1], argv[0], NULL);
                break;
            default:
                printf("Preveliki broj argumenata!\n");
                exit(1);
        }

    }
    else
    {
        wait(NULL);
    }
    return 0;
}
//ako bi bilo sa execv u procesu deteto bi bio sl kod
//char* novi_argv[argc+1];
//novi_argv[0] = argv[0];
//int j=1;
//for(int i=argc-1; i>=1; i++)
//    novi_argv[j++] = argv[i];
//novi_argv[j]=NULL;
//execv(argv[0],novi_argv);