#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <stdio.h>

int     main(int ac, char **av)
{
    int     handler;
    int     pidfils;

    switch (pidfils = fork())
    {
        case (-1) :
            break;
        case (0) :
            exit(0);
            break;
        default :
            waitpid(pidfils, &handler, 0);
            printf("%d c'est bien ça :)\n", handler);
            break;
    }
    return (0);
}
