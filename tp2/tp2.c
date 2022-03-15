/**
 *                  TP2
 *  Madet Irwin, Van Cyril, Bourennane Amine
**/
#define _DEFAULT_SOURCE
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <dirent.h>
#include <time.h>
#include <pwd.h>
#include <grp.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#define BUF_SIZE 64
#define PATH_SIZE 512
#define MODE_SIZE 11

static int  opt = 0;
static int  col_blksize = 0; /* taille de la colonne de l'option -s */
static int  col_size = 0; /* taille de la colonne de la valeur size dans -ln */
static int  col_grname = 0; /* taille de la colonne pour le nom du groupe */
static int  col_pwname = 0; /* taille de la colonne pour le nom du user */
static int  col_inode = 0; /* taille de la colonne pour l'option -i */
static int  col_link = 0; /* taille de la colonne pour le nombre de lien -ln */
typedef int (*filter)(const struct dirent *);

typedef struct  s_lst
{
    char            path[PATH_SIZE]; /* chemin du dossier */
    struct s_lst    *next; /* élément suivant de la liste */
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

char    file_type(mode_t mode)
{
    if (S_ISDIR(mode))
        return ('d');
    if (S_ISLNK(mode))
        return ('l');
    if (S_ISSOCK(mode))
        return ('s');
    if (S_ISCHR(mode))
        return ('c');
    if (S_ISBLK(mode))
        return ('b');
    if (S_ISFIFO(mode))
        return ('f');
    return ('-');
}

char    *all_mode(mode_t mode, char *ret)
{
    int     i;
    int     val;

    for (i = 0; i < 3; i++) /* parcours les 3 derniers set de 3 bits du mode */
    {
        if ((val = (mode & (7 << (i * 3))) >> i * 3)) /* récupère un set de 3 bits */
        {
            if (val & 1) /* bit #1 pour l'exécution */
                ret[((2 - i) * 3) + 3] = 'x';
            if (val & 2) /* bit #2 pour l'écriture */
                ret[((2 - i) * 3) + 2] = 'w';
            if (val & 4) /* bit #3 pour la lecture */
                ret[((2 - i) * 3) + 1] = 'r';
        }
    }
    ret[0] = file_type(mode); /* met le type de fichier dans la chaîne ret */
    return (ret);
}

int     recup_link(char *pathname, char *fname, char *buf)
{
    char    path[PATH_SIZE];
    int     size;

    snprintf(path, PATH_SIZE, "%s/%s", pathname, fname);
    if ((size = readlink(path, buf, BUF_SIZE)) == -1)
    {
        perror("Erreur dans recup_link");
        return (0);
    }
    buf[size] = 0;
    return (0);
}

int     is_not_parent(const char *path)
{
    return (strcmp(path, "..") && strcmp(path, "."));
}

int     filter_secret(const struct dirent *f)
{
    return (f->d_name[0] != '.');
}

int     filter_all(const struct dirent *f)
{
    return (1);
}

int     comp(const struct dirent **f1, const struct dirent **f2)
{
    if (!is_not_parent((*f1)->d_name) || !is_not_parent((*f2)->d_name))
        return (strcmp((*f1)->d_name, (*f2)->d_name));
    if ((*f1)->d_name[0] == '.' && (*f2)->d_name[0] != '.')
        return (strcmp(&(*f1)->d_name[1], (*f2)->d_name));
    if ((*f1)->d_name[0] != '.' && (*f2)->d_name[0] == '.')
        return (strcmp((*f1)->d_name, &(*f2)->d_name[1]));
    if ((*f1)->d_name[0] == '.' && (*f2)->d_name[0] == '.')
        return (strcmp(&(*f1)->d_name[1], &(*f2)->d_name[1]));
    return (strcmp((*f1)->d_name, (*f2)->d_name));
}

void    print_inode(struct stat sb)
{
    printf("%*ld ", col_inode, sb.st_ino);
}

int     print_list(struct stat sb, char *fname, char *pathname)
{
    struct group    *grp;
    struct passwd   *user;
    char            *time_str;
    char            buf[BUF_SIZE];
    char            ret[MODE_SIZE] = "----------"; /* chaîne des perms et type du fichier */

    time_str = ctime(&sb.st_mtime);
    time_str[strlen(time_str) - 1] = 0;
    if (S_ISLNK(sb.st_mode))
        if (recup_link(pathname, fname, buf))
            return (1);
    if (!(grp = getgrgid(sb.st_gid)))
    {
        perror("Erreur dans print_list");
        return (1);
    }
    if (!(user = getpwuid(sb.st_uid)))
    {
        perror("Erreur dans print_list");
        return (1);
    }
    printf("%s %*ld %*s %*s %*ld %s %s%s%s\n", all_mode(sb.st_mode, ret),
        col_link, sb.st_nlink, col_pwname, user->pw_name, col_grname,
        grp->gr_name, col_size, sb.st_size, time_str, fname,
        ((S_ISLNK(sb.st_mode)) ? " --> " : ""),
        ((S_ISLNK(sb.st_mode)) ? buf : ""));
    return (0);
}

int    print_num(struct stat sb, char *fname, char *pathname)
{
    char    *time_str;
    char    buf[BUF_SIZE];
    char    ret[MODE_SIZE] = "----------"; /* chaîne des perms et type du fichier */

    time_str = ctime(&sb.st_mtime);
    time_str[strlen(time_str) - 1] = 0; /* on retire le \n final */
    if (S_ISLNK(sb.st_mode))
        if (recup_link(pathname, fname, buf))
            return (1);
    printf("%s %*ld %4d %4d %*ld %s %s%s%s\n", all_mode(sb.st_mode, ret),
        col_link, sb.st_nlink, sb.st_uid, sb.st_gid,
        col_size, sb.st_size, time_str, fname,
        ((S_ISLNK(sb.st_mode)) ? " --> " : ""),
        ((S_ISLNK(sb.st_mode)) ? buf : ""));
    return (0);
}

void    print_size(struct stat sb)
{
    printf("%*ld ", col_blksize, sb.st_blocks / 2);
}

int     data_file(struct stat sb, char *fname, char *pathname)
{
    if (opt & 8)
        print_inode(sb);
    if (opt & 16)
        print_size(sb);
    if (opt & 4)
    {
        if (print_num(sb, fname, pathname))
            return (1);
    }
    else if (opt & 2)
    {
        if (print_list(sb, fname, pathname))
            return (1);
    }
    else
        printf("%s\n", fname);
    return (0);
}

int     nb_len(unsigned long int nb)
{
    int     ret;

    for (ret = 1; nb > 9; ret++, nb /= 10) {}
    return (ret);
}

void    init_col(void)
{
    col_grname = 0;
    col_pwname = 0;
    col_blksize = 0;
    col_size = 0;
    col_link = 0;
    col_inode = 0;
}

int     print_total_column_size(struct dirent **dir, int nb_file, char *pathname)
{
    struct group    *grp;
    struct passwd   *user;
    char            path[PATH_SIZE];
    struct stat     sb;
    long int        total;
    int             i;
    int             tmp;

    init_col();
    for (i = 0, total = 0; i < nb_file; i++)
    {
        snprintf(path, PATH_SIZE, "%s/%s", pathname, dir[i]->d_name);
        if (lstat(path, &sb) == -1)
        {
            perror("Erreur dans print_total_column_size");
            return (0);
        }
        total += sb.st_blocks / 2;
        if (opt & 2 && !(opt & 4))
        {
            if (!(grp = getgrgid(sb.st_gid)))
            {
                perror("Erreur dans print_total_column_size");
                return (1);
            }
            if (!(user = getpwuid(sb.st_uid)))
            {
                perror("Erreur dans print_total_column_size");
                return (1);
            }
            if ((tmp = strlen(grp->gr_name)) > col_grname)
                col_grname = tmp;
            if ((tmp = strlen(user->pw_name)) > col_pwname)
                col_pwname = tmp;
        }
        if (opt & 16 && (tmp = nb_len(sb.st_blocks / 2)) > col_blksize)
            col_blksize = tmp;
        if (opt & 6 && (tmp = nb_len(sb.st_size)) > col_size)
            col_size = tmp;
        if (opt & 6 && (tmp = nb_len(sb.st_nlink)) > col_link)
            col_link = tmp;
        if (opt & 8 && (tmp = nb_len(sb.st_ino)) > col_inode)
            col_inode = tmp;
    }
    printf("total %ld\n", total);
    return (0);
}

int     do_ls_thing(char *pathname);

int     explore(t_lst *l_path)
{
    if (!*l_path)
        return (0);
    if (explore(&(*l_path)->next))
        return (1);
    printf("\n%s:\n", (*l_path)->path);
    if (do_ls_thing((*l_path)->path))
        return (1);
    free(*l_path);
    return (0);
}

int     do_ls_thing(char *pathname)
{
    struct dirent   **cnt_dir;
    struct stat     sb;
    char            path[PATH_SIZE];
    t_lst           lst_path; /* liste des chemins des dossiers présents dans le dossier courant */
    filter          filt;
    int             ret;
    int             i;

    if (lstat(pathname, &sb) == -1)
    {
        perror("Erreur dans do_ls_thing");
        return (0);
    }
    if (!S_ISDIR(sb.st_mode) || opt & 64) /* fichier ou option d */
    {
        if (data_file(sb, pathname, ""))
            return (1);
        return (0);
    }
    filt = (opt & 32) ? filter_all : filter_secret; /* avec option a ou sans */
    if ((ret = scandir(pathname, &cnt_dir, filt, comp)) == -1)
    {
        perror("Erreur dans do_ls_thing");
        return (0);
    }
    if ((opt & 30) && print_total_column_size(cnt_dir, ret, pathname))
        return (0);
    lst_path = NULL;
    for (i = 0; i < ret; i++)
    {
        snprintf(path, PATH_SIZE, "%s/%s", pathname, cnt_dir[i]->d_name);
        if (lstat(path, &sb) == -1)
        {
            perror("Erreur dans do_ls_thing");
            return (0);
        }
        if (opt & 1 && S_ISDIR(sb.st_mode) && /* option R active et est un dosier */
            is_not_parent(cnt_dir[i]->d_name)) /* si ce n'est pas . ni .. */
            if (add_head(&lst_path, path)) /* ajout du dossier en tête de liste */
                return (1);
        if (data_file(sb, cnt_dir[i]->d_name, pathname))
            return (1);
        free(cnt_dir[i]);
    }
    free(cnt_dir);
    if (explore(&lst_path)) /* explore chaque dossier dans la liste */
        return (1);
    return (0);
}

int     main(int ac, char **av)
{
    const char  *optstring = ":iRlnsda";
    int         ret;
    int         nb_arg;
    
    while (EOF != (ret = getopt(ac, av, optstring)))
    {
        switch (ret)
        {
            case 'R':
                opt |= 1;
                break;
            case 'l':
                opt |= 2;
                break;
            case 'n':
                opt |= 4;
                break;
            case 'i':
                opt |= 8;
                break;
            case 's':
                opt |= 16;
                break;
            case 'a':
                opt |= 32;
                break;
            case 'd':
                opt |= 64;
                break;
            case '?':
                printf("unknown option %c\n", optopt);
                break;
            default:
                printf("default : unknown option %c\n", optopt);
                break;
        }
    }
    if ((nb_arg = ac - optind) == 0) /* s'effectue sur la racine */
    {
        if (do_ls_thing("."))
            return (1);
    }
    else
    {
        while (--ac >= optind)
        {
            if (!(opt & 64) && nb_arg > 1) /* ne s'affiche pas pour l'option d */
                printf("%s:\n", av[ac]);
            if (do_ls_thing(av[ac]))
                return (1);
            if (ac > optind && !(opt & 64))
                printf("\n");
        }
    }
    return (0);
}
