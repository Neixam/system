#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <ctype.h>
#define BUF_SIZE 1

void   child_death(int sig)
{
    if (sig == SIGCHLD)
    {
        printf("\nLa mort du fils\n");
        exit(0);
    }
}

int     main(int ac, char **av)
{
    int     pidfils;
    int     pipfd[2];
    int     ret;
    int     i;
    char    buf[BUF_SIZE];

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
            for (i = 0; (ret = read(pipfd[0], buf, BUF_SIZE)); i++)
            {
                if (i == 10)
                {
                    close(pipfd[0]);
                    exit(0);
                }
                if (ret == -1)
                {
                    perror("5:");
                    return (5);
                }
                buf[0] = toupper(buf[0]);
                if (write(STDOUT_FILENO, buf, BUF_SIZE) == -1)
                {
                    perror("6:");
                    return (6);
                }
            }
            break;
        default :
            signal(SIGCHLD, child_death);
            close(pipfd[0]);
            while ((ret = read(STDIN_FILENO, buf, BUF_SIZE)))
            {
                if (ret == -1)
                {
                    perror("2:");
                    return (2);
                }
                if (write(pipfd[1], buf, BUF_SIZE) == -1)
                {
                    perror("4:");
                    return (4);
                }
            }
            close(pipfd[1]);
    }
    return (0);
}
