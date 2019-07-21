#include <stdlib.h>
#include "../inc/ft_malloc.h"

#define TINY    (TINY_BLOCK)
#define SMALL   (SMALL_BLOCK)
#define LARGE   (8 * SMALL)

int main(void)
{
    size_t      Q = 10;
    size_t      i, j;
    char        *tiny[Q];
    char        *small[Q];
    char        *large[Q];

    i = 0;
    while (i < Q)
    {
        small[i] = (char *)malloc(SMALL);
        large[i] = (char *)malloc(LARGE);
        tiny[i] = (char *)malloc(TINY);
        if (small[i] == NULL || tiny[i] == NULL || large[i] == NULL)
            return (1);
        j = 0;
        while (j < LARGE)
        {
            (j < TINY) ? tiny[i][j] = (char)(j % (128 - 34) + 33) : 0;
            (j < SMALL) ? small[i][j] = (char)(j % (128 - 34) + 33) : 0;
            large[i][j] = (char)(j % (128 - 34) + 33);
            j++;
        }
        i++;
    }
	ft_putstr("orig\n");
    show_alloc_mem();

    /* Freeing chunks */
	
    i = 0;
    while (i < Q)
        free(tiny[i++]);
	ft_putstr("\nfreed tinies\n");
	show_alloc_mem();
    i = 0;
    while (i < Q)
        free(small[i++]);
	ft_putstr("\nfreed smalls\n");
	show_alloc_mem();
    i = 0;
    while (i < Q)
        free(large[i++]);
	ft_putstr("\nfreed all\n");
	show_alloc_mem();

    return 0;
}
