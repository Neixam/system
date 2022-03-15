#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <stdio.h>

int     main(int ac, char **av)
{
    struct stat s;
    char        buf[124];
    int         size;

    if (ac != 2)
    {
        fprintf(stderr, "Pas le bon nombre d'argument.\n");
        return (1);
    }
    if (lstat(av[1], &s) == -1)
    {
        perror("Erreur dans stat");
        return (2);
    }
    if (S_ISLNK(s.st_mode))
    {
        if ((size = readlink(av[1], buf, 124)) == -1)
        {
            perror("Erreur dans readlink");
            return (3);
        }
        buf[size] = 0;
    }
    printf("#inode = %ld, size = %ld, date = %ld et %c%s%s\n", 
        s.st_ino, s.st_size, s.st_mtime,
        ((S_ISDIR(s.st_mode)) ? 'd' : ((S_ISLNK(s.st_mode)) ? 'l' : ((S_ISREG(s.st_mode)) ? 'f' : '?'))),
        ((S_ISLNK(s.st_mode)) ? " --> " : ""), ((S_ISLNK(s.st_mode)) ? buf : ""));
    return (0);
}
