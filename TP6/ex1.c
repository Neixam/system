#include <signal.h>
#include <stdio.h>
#include <string.h>

void    sui(int signal)
{
    if (signal == SIGINT)
        printf("LALALALALa\n");
}

int     main(void)
{
    struct sigaction s;

    memset(&s, 0, sizeof(struct sigaction));
    s.sa_handler = sui;
    sigaction(SIGINT, &s, NULL);
    while (1);
    return (0);
}
