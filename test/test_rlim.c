#include <stdio.h>
#include <stdlib.h>
#include "../inc/ft_malloc.h"

#define TINY    (TINY_BLOCK)
#define SMALL   (SMALL_BLOCK)
#define LARGE   (8 * SMALL)

#include <sys/time.h>
#include <sys/resource.h>

void	set_limit(void)
{
	struct rlimit as;

	getrlimit(RLIMIT_AS, &as);

	as = (struct rlimit){ SMALL_BLOCK, as.rlim_max };
	setrlimit(RLIMIT_AS, &as);

	getrlimit(RLIMIT_AS, &as);
	system("ulimit -a");
}

int		main(int argc, char *argv[])
{
    size_t      j;
    char        *dptr;

	(void)argc;
    dptr = (char *)malloc(LARGE);
    if (dptr == NULL)
        return (1);
    j = 0;
    while (j < LARGE)
    {
        dptr[j] = (char)(j % (128 - 34) + 33);
        j++;
    }
    show_alloc_mem();
	free(dptr);
    show_alloc_mem();

	set_limit();
	dptr = (char *)malloc(LARGE);
	if (dptr == NULL)
		return (-42);
	show_alloc_mem();
    /* Freeing chunks */
    free(dptr);
    show_alloc_mem();
    return 0;
}
