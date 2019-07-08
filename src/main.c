#include "ft_malloc.h"

void        *ft_malloc(size_t size)
{
    t_meta_blk  b;
    void        *p;
    size_t      blk_size;

    /* b = sbrk(0); */
    /* p = sbrk(sizeof(struct s_meta_blk) + size); */
    b = mmap(NULL, 0, FT_PROT_RW, FT_MAP_DEF, -1, 0);
//    p = mmap(sizeof(struct s_meta_blk) + size);
    if (p == (void *)-1)
        return (NULL);
    b->size = size;
    b->next = p;
    return (p);
}

#include <stdio.h>
#include <stdlib.h>

int         main(int argc, char **argv)
{
    void    *ptr;

    if (argc > 1)
    {
        ptr = ft_malloc(atoi(argv[1]));
        printf("ptr = %p\n", ptr);
    }
    return (0);
}
