#define _DEFAULT_SOURCE
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <dirent.h>
#include <time.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#define BUF_SIZE 64
#define PATH_SIZE 512

typedef struct  s_lst
{
    char            path[PATH_SIZE];
    struct s_lst    *next;
}               t_cell,
                *t_lst;

t_lst   new_cell(char *path)
{
    t_lst   ret;

    if (!(ret = (t_lst) malloc(sizeof(t_cell))))
    {
        perror("Erreur dans new_cell");
        return (NULL);
    }
    strcpy(ret->path, path);
    ret->next = NULL;
    return (ret);
}

int     add_head(t_lst *head, char *path)
{
    t_lst   tmp;

    tmp = *head;
    if (!(*head = new_cell(path)))
        return (1);
    (*head)->next = tmp;
    return (0);
}

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

int     do_ls_thing(char *pathname, int opt)
{
    DIR             *dir;
    struct dirent   *cnt_dir;
    struct stat     sb;
    char            path[PATH_SIZE];
    t_lst           lst_path;
    t_lst           tmp;

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
    lst_path = NULL;
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
            if (opt && S_ISDIR(sb.st_mode))
                if (add_head(&lst_path, path))
                    return (1);
            if (data_file(sb, cnt_dir->d_name, pathname))
                return (1);
        }
    }
    closedir(dir);
    while (lst_path)
    {
        printf("\n%s:\n", lst_path->path);
        do_ls_thing(lst_path->path, opt);
        tmp = lst_path;
        lst_path = lst_path->next;
        free(tmp);
    }
    return (0);
}

int     main(int ac, char **av)
{
    int     ret;
    int     opt;
    int     nb_opt;

    if (ac == 1)
    {
        if (do_ls_thing(".", 0))
            return (1);
    }
    else
    {
        opt = 0;
        nb_opt = 0;
        while ((ret = getopt(ac, av, "R")) != -1)
        {
            nb_opt++;
            if (ret == 'R')
                opt = 1;
        }
        while (--ac > 0)
        {
            if (av[ac][0] != '-')
            {
                printf("%s:\n", av[ac]);
                if (do_ls_thing(av[ac], opt))
                    return (1);
                if (ac > 1)
                    printf("\n");
            }
        }
    }
    return (0);
}
