#include <unistd.h>
#include <stdio.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#define BUF_SIZE 256

int     do_cat(char *path)
{
    int     fd;
    char    buf[BUF_SIZE];
    int     size;

    if (!path)
        fd = STDIN_FILENO;
    else
    {
        if ((fd = open(path, O_RDONLY)) == -1)
        {
            perror("Error in do_cat");
            return (1);
        }
    }
    while ((size = read(fd, buf, BUF_SIZE)))
        write(STDOUT_FILENO, buf, size);
    if (fd != STDIN_FILENO)
    {
        if (close(fd) == -1)
        {
            perror("Error in do_cat");
            return (1);
        }
    }
    return (0);
}

int     main(int ac, char **av)
{
    if (ac == 1)
    {
        if (do_cat(NULL))
            return (1);
    }
    else
    {
        while (--ac)
            if (do_cat(av[ac]))
                return (1);
    }
    return (0);
}
