#define _DEFAULT_SOURCE
#include <sys/time.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>
#define BUF_SIZ 256

int     main(int ac, char **av)
{
    struct
    {
        int     pipe[2];
        int     pid;
        char    arg1[64];
        char    arg2[64];
    }       exec[3];
    int     i;
    int     size;
    int     bigfd;
    int     selection;
    fd_set  set;
    char    buff[BUF_SIZ];

    for (i = 0, bigfd = 0; i < 3; i++)
    {
        if (pipe(exec[i].pipe) == -1)
        {
            perror("pipe");
            return (3);
        }
        if (exec[i].pipe[1] > bigfd)
            bigfd = exec[i].pipe[1];
        sprintf(exec[i].arg1, "Je suis le %d.", i + 1);
        sprintf(exec[i].arg2, "%d", (i + 1) * 1000000);
    }
    for (i = 0; i < 3; i++)
    {
        switch (exec[i].pid = fork())
        {
            case (-1) :
                perror("fork");
                return (2);
            case (0) :
                if (dup2(exec[i].pipe[1], 1) == -1)
                {
                    perror("dup2");
                    return (4);
                }
                close(exec[i].pipe[0]);
                if (execlp("./repeat", "./repeat", exec[i].arg1, exec[i].arg2, NULL) == -1)
                {
                    perror("execlp");
                    return (1);
                }
            default :
                close(exec[i].pipe[1]);
        }
    }
    while (1)
    {
        FD_ZERO(&set);
        for (i = 0; i < 3; i++)
            FD_SET(exec[i].pipe[0], &set);
        if ((selection = select(bigfd + 1, &set, NULL, NULL, NULL)) <= 0)
        {
            perror("select");
            return (6);
        }
        for (i = 0; i < 3; i++)
        {
            if (FD_ISSET(exec[i].pipe[0], &set))
            {
                if ((size = read(exec[i].pipe[0], buff, BUF_SIZ)) == -1)
                {
                    perror("read");
                    return (7);
                }
                if (write(1, buff, size) == -1)
                {
                    perror("write");
                    return (8);
                }
            }
        }
    }
    return (0);
}
