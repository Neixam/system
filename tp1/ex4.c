#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <stdio.h>

int     main(int ac, char **av)
{
    struct stat sb1;
    struct stat sb2;
    int         b;

    if (ac != 2)
    {
        fprintf(stderr, "Mauvais nombre d'argument.\n");
        return (1);
    }
    if (lstat(av[1], &sb1) == -1)
    {
        perror("Appelle lstat");
        return (2);
    }
    b = 0;
    do
    {
        if (b)
            sleep(1);
        if (lstat(av[1], &sb2) == -1)
        {
            perror("Appelle lstat");
            return (3);
        }
        b = 1;
    } while (sb1.st_mtime == sb2.st_mtime);
    printf("%s a été modifié.\n", av[1]);
    return (0);
}
