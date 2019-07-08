#include "ft_malloc.h"

void            init_area(void *ptr, size_t res_size, size_t initial_size)
{
    t_meta_blk  blk;

    blk = (t_meta_blk)ptr;
    if (initial_size <= TINY_BLOCK)
        blk->type = TINY;
    else if (initial_size <= SMALL_BLOCK)
        blk->type = SMALL;
    else
        blk->type = LARGE;
    blk->size = res_size - sizeof(struct s_meta_blk);
    blk->data = blk + sizeof(struct s_meta_blk);
    blk->free = false;
    blk->prev = NULL;
    blk->next = NULL;
}

void            *allocate_area(size_t size)
{
    void        *ptr;

    ptr = mmap(NULL, size, FT_PROT_RW, FT_MAP_DEF, -1, 0);
    if (ptr == MAP_FAILED)
        return (NULL);
    return (ptr);
}

void            *get_new_area(size_t initial_size)
{
    void        *ptr;
    size_t      alloc_size;

    if (initial_size <= SMALL_BLOCK)
        alloc_size = get_alloc_size(initial_size);
    else
        alloc_size = initial_size + sizeof(struct s_meta_blk);
    ptr = allocate_area(alloc_size);
    if (ptr != NULL)
        init_area(ptr, alloc_size, initial_size);
    return (ptr);
}

unsigned        get_pages_amount(size_t page_size, size_t block_size)
{
    unsigned    num_pages;
    unsigned    num_allocs;

    num_allocs = 0;
    num_pages = 0;

    //!!! if block_size && page_size > 0
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

    b = (t_meta_blk)g_addr;
    if (b == NULL)
        p = get_new_area(size);    
    return (p);
}

#include <stdio.h>
#include <stdlib.h>

int         main(int argc, char **argv)
{
    void    *ptr;
    size_t  size;

    if (argc > 1)
        size = atoi(argv[1]);
    else
        return -1;
    ptr = ft_malloc(size);
    printf("ptr = %p\n", ptr);
    printf("Pagesize %d\n", getpagesize());
    printf("Type: %d\n", ((t_meta_blk)ptr)->type);
    munmap(ptr, ((t_meta_blk)ptr)->size + sizeof(struct s_meta_blk));

    return (0);
}
