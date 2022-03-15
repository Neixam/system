#include <arpa/inet.h>
#include <sys/types.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <sys/socket.h>
#define PORT 4035
#define BUF_SIZE 256

typedef struct sockaddr_in saddi_t;

saddi_t rempli_add(int port)
{
    saddi_t ret;

    if (port <= 1024)
    {
        fprintf(stderr, "Eh va y nik frérot t'es fou ? Reviens ac plus que ça\n");
        _exit(1);
    }
    ret.sin_family = AF_INET;
    ret.sin_port = htons(port);
    ret.sin_addr.s_addr = INADDR_ANY;
    return (ret);
}

int     create_socket(void)
{
    int     ret;

    if ((ret = socket(AF_INET, SOCK_STREAM, 0)) == -1)
    {
        perror("error in call socket");
        _exit(2);
    }
    return (ret);
}

saddi_t build_socket(int port, int sfd)
{
    saddi_t ret;

    ret = rempli_add(PORT);
    if (bind(sfd, (struct sockaddr *) &ret, sizeof(saddi_t)) == -1)
    {
        perror("error in call bind");
        _exit(3);
    }
    if (listen(sfd, 42) == -1)
    {
        perror("error in call listen");
        _exit(4);
    }
    return (ret);
}

int     ft_fdputstr(int fd, char *s)
{
    return (write(fd, s, strlen(s)));
}

int     main(int ac, char **av)
{
    saddi_t     saddr, paddr;
    socklen_t   size_paddr;
    int         sfd;
    int         cfd;

    sfd = create_socket();
    saddr = build_socket(PORT, sfd);
    while (1)
    {
        size_paddr = sizeof(saddi_t);
        if ((cfd = accept(sfd, (struct sockaddr *) &paddr, &size_paddr)) == -1)
        {
            perror("error in call accept");
            _exit(5);
        }
        if (ft_fdputstr(cfd, "Bonjour\n") == -1)
        {
            perror("error in call write");
            _exit(7);
        }
    }
    return (0);
}
