#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <stdio.h>
#include <stdlib.h>

int     main(void)
{
    int     ret;

    switch (ret = fork())
    {
        case (-1) :
            perror("Error in fork:");
            return (1);
            break;
        case (0) :
            if (execlp("ls", "ls", "-l", "/", NULL) == -1)
            {
                perror("Error in execlp:");
                return (1);
            }
            break;
        default :
            wait(NULL);
            printf("ls est terminé\n");
            break;
    }
    return (0);
}
