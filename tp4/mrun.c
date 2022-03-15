#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

int     main(int ac, char **av, char **env)
{
    char    name[5] = "RUN_0";
    char    *prog;

    while ((prog = getenv(name)))
    {
        switch (fork())
        {
            case (-1) :
                perror("Error in fork:");
                return (1);
                break;
            case (0) :
                if (execve(prog, av, env) == -1)
                {
                    perror("Error in execvp:");
                    return (1);
                }
                break;
            default :
                wait(NULL);
                break;
        }
        name[4]++;
    }
}
