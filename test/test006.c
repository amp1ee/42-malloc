#include <stdlib.h>
#include "../inc/ft_malloc.h"

#define TINY    (TINY_BLOCK)
#define SMALL   (SMALL_BLOCK)
#define LARGE   (8 * SMALL)

int main(void)
{
    char        *tiny;
    
	tiny = malloc(-100);

	printf("size-t is: %zu\n", (size_t)-100);

	if (tiny == NULL)
		return (-42);

    free(tiny);
	ft_putstr("\nfreed\n");
	show_alloc_mem();

    return 0;
}
