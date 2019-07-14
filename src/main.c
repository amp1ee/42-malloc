#include "ft_malloc.h"

#include <stdio.h>
#include <stdlib.h>

int         main(int argc, char **argv)
{
    int     q = 24;
    void    *ptr[q];
    size_t  size;

    if (argc > 1)
        size = ALIGN4(atoi(argv[1]));
    else
        return -1;
    int i = 0;
    while (i < q)
    {
        ptr[i] = ft_malloc(size  );
		if (!ptr[i])
			return -1;
        printf("ptr[%i] = %p\n", i, ptr[i]);
        void *this = ptr[i];
        printf("Size: %zu\n", (((t_block)(this - sizeof(struct s_block)))->size));
        (i % 3) ? ft_free(ptr[i]) : 0;
        i++;
    }
    // munmap(ptr, ((t_area)ptr)->size + sizeof(struct s_block));

    return (0);
}
