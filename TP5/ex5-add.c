#include <stdio.h>

int     main(void)
{
    int     x, y;

    setvbuf(stdout, NULL, _IOFBF, 0);
    do
    {
        scanf("%d %d", &x, &y);
        printf("%d\n", x + y);
    } while (1);
    return (0);
}
