#define _DEFAULT_SOURCE
#include <unistd.h>
#include <stdio.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>

#define BUF_SIZE 256

static int                  opt = 0;
static unsigned long int    total_line = 0;
static unsigned long int    total_char = 0;
static unsigned long int    total_wd = 0;

int     is_alpha(int c)
{
    return (c != '\t' && c != '\n' && c != ' ' && c != 127);
}

int     count_word(char *s)
{
    int         ret;
    int         i;
    static int  n_f = 0;

    for (ret = 0, i = 0; s[i]; i++)
    {
        if (is_alpha(s[i]))
        {
            for (; s[i] && is_alpha(s[i]) ; i++) {}
            if (!n_f)
            {
                ret++;
                if (!s[i])
                    n_f = 1;
            }
        }
        else
        {
            if (n_f)
                n_f = 0;
        }
    }
    return (ret);
}

int     count_line(char *s)
{
    int     ret;
    int     i;

    for (ret = 0, i = 0; s[i]; i++)
        if (s[i] == '\n')
            ret++;
    return (ret);
}

int     do_wc(char *path)
{
    char            buf[BUF_SIZE + 1];
    int             fd;
    int             size;
    unsigned int    nb_char;
    unsigned int    nb_word;
    unsigned int    nb_line;

    if (!path)
        fd = STDIN_FILENO;
    else
    {
        if ((fd = open(path, O_RDONLY)) == -1)
        {
            perror("Error in do_wc");
            return (1);
        }
    }
    for (nb_char = 0, nb_word = 0, nb_line = 0; (size = read(fd, buf, BUF_SIZE));)
    {
        if (size == -1)
        {
            perror("Error in do_wc");
            return (1);
        }
        nb_char += size;
        buf[size + 1] = 0;
        if (opt & 4 || opt == 0)
            nb_word += count_word(buf);
        if (opt & 2 || opt == 0)
            nb_line += count_line(buf);
    }
    if (close(fd) == -1)
    {
        perror("Error in do_wc");
        return (1);
    }
    total_line += nb_line;
    total_wd += nb_word;
    total_char += nb_char;
    if (opt & 4 || opt == 0)
        printf("%d ", nb_line);
    if (opt & 2 || opt == 0)
        printf("%d ", nb_word);
    if (opt & 1 || opt == 0)
        printf("%d ", nb_char);
    return (0);
}

int     main(int ac, char **av)
{
    const char  *optstring = ":clw";
    int         ret;
    int         i;
    
    while (EOF != (ret = getopt(ac, av, optstring)))
    {
        switch (ret)
        {
            case 'c':
                opt |= 1;
                break;
            case 'l':
                opt |= 2;
                break;
            case 'w':
                opt |= 4;
                break;
            case '?':
                printf("unknown option %c\n", optopt);
                break;
            default:
                printf("default : unknown option %c\n", optopt);
                break;
        }
    }
    if (ac - optind == 0)
    {
        if (do_wc(NULL))
            return (1);
    }
    else
    {
        for (i = optind; i < ac; i++)
        {
            if (do_wc(av[i]))
                return (1);
            printf("%s\n", av[i]);
        }
        if (i > 1)
        {
            if (opt & 4 || opt == 0)
                printf("%ld ", total_line);
            if (opt & 2 || opt == 0)
                printf("%ld ", total_wd);
            if (opt & 1 || opt == 0)
                printf("%ld ", total_char);
            printf("total :)\n");
        }
    }
    return (0);
}
