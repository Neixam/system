#include <unistd.h>
#include <stdio.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>

int     main(int ac, char **av)
{
    int     fd;
    int     fdnew;

    if (ac != 2)
    {
        write(STDERR_FILENO, "Not enought argument\n", 21);
        return (1);
    }
    if (close(STDOUT_FILENO) == -1)
    {
        perror("ERROR");
        return (1);
    }
    if ((fd = open(av[1], O_CREAT | O_WRONLY | O_TRUNC,
        (S_IRWXU & (~S_IXUSR)) | S_IROTH | S_IRGRP) == -1))
        return (1);
    fdnew = dup(fd);
    printf("NIKKKK\n");
    close(fdnew);
    close(fd);
    return (0);
}
