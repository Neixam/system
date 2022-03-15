#include <sys/types.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <ctype.h>
#include <string.h>
#define BUF_SIZE 64

int     main(void)
{
    char    s[BUF_SIZE];
    char    s2[BUF_SIZE];
    int     pipe_add[2];
    int     pipe_mul[2];
    int     pipe_sub[2];
    int     pipe_dis[2];
    int     size;

    if (pipe(pipe_add) == -1)
    {
        perror("Error in the first pipe:");
        return (1);
    }
    if (pipe(pipe_add) == -1)
    {
        perror("Error in the second pipe:");
        return (2);
    }
    if (pipe(pipe_mul) == -1)
    {
        perror("Error in the thirst pipe:");
        return (3);
    }
    if (pipe(pipe_dis) == -1)
    {
        perror("Error in the fourth pipe:");
        return (4);
    }
    switch (fork())
    {
        case (-1) :
            perror("Error in first fork:");
            return (5);
        case (0) :
            close(pipe_mul[1]);
            close(pipe_mul[0]);
            close(pipe_sub[1]);
            close(pipe_sub[0]);
            dup2(pipe_add[0], 0);
            dup2(pipe_dis[1], 1);

            execlp("./add", "./add", NULL);
            break;
        default :
            switch (fork())
            {
                case (-1) :
                    perror("Error in second fork:");
                    return (6);
                case (0) :
                    close(pipe_add[1]);
                    close(pipe_add[0]);
                    close(pipe_sub[1]);
                    close(pipe_sub[0]);
                    dup2(pipe_mul[0], 0);
                    dup2(pipe_dis[1], 1);
                    execlp("./mul", "./mul", NULL);
                    break;
                default :
                    switch (fork())
                    {
                        case (-1) :
                            perror("Error in thirst fork:");
                            return (7);
                        case (0) :
                            close(pipe_add[1]);
                            close(pipe_add[0]);
                            close(pipe_mul[0]);
                            close(pipe_mul[1]);
                            dup2(pipe_sub[0], 0);
                            dup2(pipe_dis[1], 1);
                            execlp("./sub", "./sub", NULL);
                            break;
                        default :
                            do
                            {
                                scanf("%s", s);
                                if (strcmp(s, "addition") == 0)
                                {
                                    scanf("%s", s);
                                }
                                else if (strcmp(s, "soustraction") == 0)
                                {
                                    scanf("%s", s);
                                    size = read(pipe_sub[1], s, BUF_SIZE);
                                }
                                else if (strcmp(s, "multiplication") == 0)
                                {
                                    scanf("%s", s);
                                    size = read(pipe_mul[1], s, BUF_SIZE);
                                }
                                else
                                {
                                    printf("C PAS BIEN !! C PAS BIEN !!\n");
                                }
                            } while (1);
                            break;
                    }
                    break;
            }
            break;
    }
    return (0);
}
