#include <stdio.h>
#include <stdlib.h>
#include "../inc/ft_malloc.h"
#include <time.h>

#define TINY    (TINY_BLOCK)
#define SMALL   (SMALL_BLOCK)
#define LARGE   (8 * SMALL)

void	self_mem_usage(char *self_name)
{
	char	cmd_buf[256];

	fflush(stdout);
	sprintf(cmd_buf, "ps ef -o command,rss | grep %s | grep -v grep", self_name);
	system(cmd_buf);
}

int main(int argc, char *argv[])
{
    size_t      Q = 14;
    size_t      i;
    char        *dptr[Q];
	int			r;

	(void)argc;
	srand(time(NULL));

    i = 0;
	dptr[0] = malloc(LARGE/2);
	dptr[0] = realloc(&i, LARGE /2);
	dptr[0] = realloc(&i, 0);
	dptr[0] = realloc(NULL, LARGE/2);
    while (i < Q)
    {
		r = rand();
		size_t r_size = r % LARGE;
        dptr[i] = (char *)malloc(r_size);
		if (r_size < LARGE / 3)
			dptr[i] = realloc(dptr[i], r_size / 2);
        if (dptr[i] == NULL)
            return (1);
        i++;
    }

	self_mem_usage(argv[0]);
    show_alloc_mem();

    /* Freeing chunks */
    i = 0;
    printf("========= Cleanup:\n");
    while (i < Q)
	{
        free(dptr[i++]);
	}
	self_mem_usage(argv[0]);
    return 0;
}
