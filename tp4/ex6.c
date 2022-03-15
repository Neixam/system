#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <stdio.h>

void    ft_putchar(char c)
{
    write(1, &c, 1);
}

void    ft_putnbr(int n)
{
    if (n < 10)
        return (ft_putchar(n + '0'));
    ft_putnbr(n / 10);
    ft_putnbr(n % 10);
}

void    ft_putstr(char *s)
{
    for (;*s; s++)
        ft_putchar(*s);
}

int     main(int ac, char **av)
{
    int     pid = getpid();
    int     pidfils;

    printf("mon PID est %d", pid);
    ft_putstr("my PID is ");
    ft_putnbr(pid);
    switch (pidfils = fork())
    {
        case (-1) :
            perror("ELFDK");
            break;
        case (0) :
            printf("je suis le fils et mon PID est %d", getpid());
            ft_putstr("I am the child process and my PID is ");
            ft_putnbr(getpid());
            exit(0);
            break;
        default :
            waitpid(pidfils, NULL, 0);
            printf("je suis le père et mon PID est %d", pid);
            ft_putstr("I am the parent process and my PID is ");
            ft_putnbr(getpid());
            break;
    }
    ft_putchar('\n');
    return (0);
}
