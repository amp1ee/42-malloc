#include <stdio.h>
#include <stdlib.h>
#include "../inc/ft_malloc.h"

#define TINY    (1024)
#define SMALL   (64 * TINY)
#define LARGE   (2 * SMALL)

int main(void)
{
    size_t      Q = 1;
    size_t      i, j;
    char        *tiny[Q];
    char        *small[Q];
    char        *large[Q];

    i = 0;
    while (i < Q)
    {
        small[i] = (char *)ft_malloc(SMALL);
        large[i] = (char *)ft_malloc(LARGE);
        tiny[i] = (char *)ft_malloc(TINY);
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

    printf("TINY:\n%s\n", tiny[i - 1]);
  //printf("SMALL:\n%s\n", small[i - 1]);

    show_alloc_mem();

    /* Freeing chunks */

    i = 0;
    printf("========= Tiny cleanup:\n");
    while (i < Q)
        ft_free(tiny[i++]);

    i = 0;
    printf("========= Small cleanup:\n");
    while (i < Q)
        ft_free(small[i++]);

    i = 0;
    printf("========= Large cleanup:\n");
    while (i < Q)
        ft_free(large[i++]);

    return 0;
}
