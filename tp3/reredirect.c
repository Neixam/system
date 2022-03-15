#include <unistd.h>
#include <stdio.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>

int     main(int ac, char **av)
{
    int     fd;
    int     fdnew;
    char    in[25];

    if (ac != 2)
    {
        write(STDERR_FILENO, "Not enought argument\n", 21);
        return (1);
    }
    if (close(STDIN_FILENO) == -1)
    {
        perror("ERROR");
        return (1);
    }
    if ((fd = open(av[1], O_RDONLY) == -1))
        return (1);
    fdnew = dup(fd);
    scanf("%s", in);
    printf("J'ai lu %s\nQuelle dinguerie quand même !\n", in);
    close(fdnew);
    close(fd);
    return (0);
}
