#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>

int     main(int ac, char **av)
{
    int     pidfils;
    int     pipfd[2];

    if (ac != 3)
    {
        fprintf(stderr, "C PAS BIEN C PAS BIEN\n");
        return (7);
    }
    if (pipe(pipfd) == -1)
    {
        perror("LAOLL");
        return (3);
    }
    switch (pidfils = fork())
    {
        case (-1) :
            perror("Nik");
            return (1);
        case (0) :
            if (dup2(pipfd[1], STDOUT_FILENO) == -1)
            {
                perror("LSLDMQ");
                return (5);
            }
            if (execlp(av[1], av[1], NULL) == -1)
            {
                perror("nik2");
                return (2);
            }
            exit(0);
            break;
        default :
            if (dup2(pipfd[0], STDIN_FILENO) == -1)
            {
                perror("dlfkldk");
                return (4);
            }
            close(pipfd[1]);
            if (execlp(av[2], av[2], NULL) == -1)
            {
                perror("nik3");
                return (6);
            }
    }
    return (0);
}
