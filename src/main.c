#include "ft_malloc.h"

void            *get_new_block(size_t size)
{
    void        *ptr;
    size_t      alloc_size;

    alloc_size = get_alloc_size(size);
    ptr = mmap(NULL, alloc_size, FT_PROT_RW, FT_MAP_DEF, -1, 0);
    if (ptr == MAP_FAILED)
        return (NULL);
    return (ptr);
}

unsigned        get_pages_amount(size_t page_size, size_t block_size)
{
    unsigned    num_pages;
    unsigned    num_allocs;

    num_allocs = 0;
    num_pages = 0;
    // if block_size && page_size > 0
    while (num_allocs < MIN_NUM_ALLOCS)
    {
        num_pages++;
        num_allocs += num_pages * page_size / block_size;
    }
    return (num_pages);
}

size_t          get_alloc_size(size_t size)
{
    size_t      alloc_size;
    size_t      page_size;
    unsigned    pages_num;

    page_size = getpagesize();
    pages_num = get_pages_amount(page_size, size + sizeof(struct s_meta_blk));
    alloc_size = pages_num * page_size;
    return (alloc_size);
}

void            *ft_malloc(size_t size)
{
    t_meta_blk  b;
    void        *p;
//    size_t      blk_size;

    b = (t_meta_blk)g_addr;
    if (b == NULL)
        p = get_new_block(size);
    
//    p = NULL;
//    p = mmap(sizeof(struct s_meta_blk) + size);
//    if (p == (void *)-1)
//        return (NULL);
//    b->size = size;
//    b->next = p;
    return (p);
}

#include <stdio.h>
#include <stdlib.h>

int         main(int argc, char **argv)
{
    void    *ptr;
    size_t  size;

    if (argc > 1)
//        size = ALIGN4(atoi(argv[1]));
        size = atoi(argv[1]);
    else
        return -1;
    ptr = ft_malloc(size);
    printf("ptr = %p\n", ptr);
    printf("Pagesize %d\n", getpagesize());
 
    return (0);
}
