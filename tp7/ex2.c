#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>

int     main(int ac, char **av)
{
    int ret;

    ret = socket(AF_INET, SOCK_STREAM, 0);
    return (0);
}
