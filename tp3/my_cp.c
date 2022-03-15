#include <unistd.h>
#include <stdio.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#define BUF_SIZE 256

int     do_cp(char *src, char *dest)
{
    char    buf[BUF_SIZE];
    int     fd_src;
    int     fd_dest;
    int     size;

    if ((fd_src = open(src, O_RDONLY)) == -1)
    {
        perror("Error in do_cp");
        return (1);
    }
    if ((fd_dest = open(dest, O_WRONLY | O_CREAT | O_TRUNC,
        (S_IRWXU & (~S_IXUSR)) | S_IROTH | S_IRGRP)) == -1)
    {
        perror("Error in do_cp");
        return (1);
    }
    while ((size = read(fd_src, buf, BUF_SIZE)))
        write(fd_dest, buf, size);
    if (close(fd_src) == -1 || close(fd_dest) == -1)
    {
        perror("Error in do_cp");
        return (1);
    }
    return (0);
}

int     main(int ac, char **av)
{
    if (ac != 3)
    {
        write(STDERR_FILENO, "Not enought argument\n", 21);
        return (1);
    }
    else
    {
        if (do_cp(av[1], av[2]))
            return (1);
    }
    return (0);
}
