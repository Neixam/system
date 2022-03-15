#define _DEFAULT_SOURCE
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <dirent.h>
#include <time.h>
#include <stdio.h>
#include <string.h>
#define BUF_SIZE 64
#define PATH_SIZE 512

int     data_file(struct stat sb, char *fname, char *pathname)
{
    int     size;
    char    type;
    char    *time_str;
    char    buf[BUF_SIZE];
    char    path[PATH_SIZE];

    time_str = ctime(&sb.st_mtime);
    time_str[strlen(time_str) - 1] = 0;
    type = ((S_ISDIR(sb.st_mode)) ? 'd' : (S_ISLNK(sb.st_mode) ? 'l' : (S_ISREG(sb.st_mode) ? 'f' : '?')));
    if (S_ISLNK(sb.st_mode))
    {
        snprintf(path, PATH_SIZE, "%s/%s", pathname, fname);
        if ((size = readlink(path, buf, BUF_SIZE)) == - 1)
        {
            perror("Erreur dans data_file");
            return (1);
        }
        buf[size] = 0;
    }
    printf("%8ld %c %8ld %s %s%s%s\n", sb.st_ino, type, sb.st_size, time_str, fname,
        ((S_ISLNK(sb.st_mode)) ? " --> " : ""),
        ((S_ISLNK(sb.st_mode)) ? buf : ""));
    return (0);
}

int     do_ls_thing(char *pathname)
{
    DIR             *dir;
    struct dirent   *cnt_dir;
    struct stat     sb;
    char            path[PATH_SIZE];

    if (!(dir = opendir(pathname)))
    {
        if (lstat(pathname, &sb) == -1)
        {
            perror("Erreur dans do_ls_thing");
            return (1);
        }
        if (data_file(sb, pathname, ""))
            return (1);
        return (0);
    }
    while ((cnt_dir = readdir(dir)))
    {
        snprintf(path, PATH_SIZE, "%s/%s", pathname, cnt_dir->d_name);
        if (cnt_dir->d_name[0] != '.')
        {
            if (lstat(path, &sb) == -1)
            {
                perror("Erreur dans do_ls_thing");
                return (1);
            }
            if (data_file(sb, cnt_dir->d_name, pathname))
                return (1);
        }
    }
    closedir(dir);
    return (0);
}

int     main(int ac, char **av)
{
    if (ac == 1)
    {
        if (do_ls_thing("."))
            return (1);
    }
    else
    {
        while (--ac > 0)
        {
            printf("%s:\n", av[ac]);
            if (do_ls_thing(av[ac]))
                return (1);
            if (ac > 1)
                printf("\n");
        }
    }
    return (0);
}
