#include <stdio.h>
#include <stdlib.h>
#include "../inc/ft_malloc.h"
#include <time.h>

#define TINY    (1024)
#define SMALL   (64 * TINY)
#define LARGE   (2 * SMALL)

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
    size_t      i, j;
    char        *dptr[Q];
	int			r;

	(void)argc;
	srand(time(NULL));

    i = 0;
	dptr[0] = ft_malloc(LARGE/2);
	dptr[0] = ft_realloc(&i, LARGE /2);
	dptr[0] = ft_realloc(&i, 0);
	dptr[0] = ft_realloc(NULL, LARGE/2);
    while (i < Q)
    {
		r = rand();
		size_t r_size = r % LARGE;
        dptr[i] = (char *)ft_malloc(r_size);
		if (r_size < LARGE / 3)
			dptr[i] = ft_realloc(dptr[i], r_size / 2);
        if (dptr[i] == NULL)
            return (1);
        j = 0;
        while (j < r_size)
        {
            dptr[i][j] = (char)(j % (128 - 34) + 33);
            j++;
        }
        i++;
    }

	self_mem_usage(argv[0]);
    show_alloc_mem();

    /* Freeing chunks */
    i = 0;
    printf("========= Cleanup:\n");
    while (i < Q)
	{
        ft_free(dptr[i++]);
	}
	self_mem_usage(argv[0]);
    return 0;
}
