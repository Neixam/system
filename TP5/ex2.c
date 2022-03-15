#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <ctype.h>
#define BUF_SIZE 1

int     main(int ac, char **av)
{
    int     pidfils;
    int     pipfd[2];
    int     ret;
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
            while ((ret = read(pipfd[0], buf, BUF_SIZE)))
            {
                if (ret == -1)
                {
                    perror("5");
                    return (5);
                }
                buf[0] = toupper(buf[0]);
                if (write(STDOUT_FILENO, buf, 1) == -1)
                {
                    perror("6");
                    return (6);
                }
            }
            exit(0);
            break;
        default :
            close(pipfd[0]);
            while ((ret = read(STDIN_FILENO, buf, BUF_SIZE)))
            {
                if (ret == -1)
                {
                    perror("2");
                    return (2);
                }
                if (write(pipfd[1], buf, BUF_SIZE) == -1)
                {
                    perror("4");
                    return (4);
                }
            }
    }
    return (0);
}
