#define _DEFAULT_SOURCE
#include <unistd.h>
#include <stdlib.h>

int     main(int ac, char **av)
{
    int     delay;
    int     len;

    if (ac != 3)
        return (1);
    delay = atoi(av[2]);
    for (len = 0; av[1][len]; len++);
    while (1)
    {
        write(STDOUT_FILENO, av[1], len);
        write(STDOUT_FILENO, "\n", 1);
        usleep(delay);
    }
    return (0);
}
