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
    int     pidfils;
    int     pidpapa;
    int     n;

    pidpapa = getpid();
    for (n = 0; n < 20; n++)
    {
        switch (pidfils = fork())
        {
            case (-1) :
                perror("Error in fork:");
                return (1);
                break;
            case (0) :
                pidfils = getpid();
                ft_putstr("je suis le numéro ");
                ft_putnbr(n + 1);
                ft_putstr(", mon PID est ");
                ft_putnbr(pidfils);
                ft_putstr(" et mon père ");
                ft_putnbr(pidpapa);
                ft_putchar('\n');
                exit(0);
                break;
            default :
                waitpid(pidfils, NULL, 0);
                ft_putstr("le processus n°");
                ft_putnbr(n + 1);
                ft_putstr(" de PID ");
                ft_putnbr(pidfils);
                ft_putstr(" est stoppé\n");
                break;
        }
    }
    for (;;)
        pause();
    return (0);
}
