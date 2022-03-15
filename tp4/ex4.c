#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <stdio.h>

int     main(int ac, char **av)
{
    int     i;

    i = 4;
    switch (fork())
    {
        case (-1) :
            perror("OUI Y A ERROR");
            return (1);
            break;
        case (0) :
            printf("avt fils dit => adresse de i = %p, la valeur de i = %d\n", &i, i);
            i = 28;
            printf("ap fils dit => adresse de i = %p, la valeur de i = %d\n", &i, i);
            exit(0);
            break;
        default :
            printf("avt père dit => adresse de i = %p, la valeur de i = %d\n", &i, i);
            i = 4845;
            printf("ap père dit => adresse de i = %p, la valeur de i = %d\n", &i, i);
            wait(NULL);
            break;
    }
    return (0);
}
