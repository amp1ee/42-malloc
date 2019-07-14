#include "ft_malloc.h"

void            init_area(void *ptr, size_t res_size, size_t initial_size)
{
    t_area      area;

    area = (t_area)ptr;
    if (initial_size <= TINY_BLOCK)
        area->type = TINY;
    else if (initial_size <= SMALL_BLOCK)
        area->type = SMALL;
    else
        area->type = LARGE;
    area->size = res_size;
    area->first_block = ptr + sizeof(struct s_area);
    area->curr_area = ptr;
    area->prev_area = NULL;
    area->next_area = NULL;
    area->size = res_size;
}

void            *allocate_area(size_t size)
{
    void        *ptr;

#ifdef DEBUG
	printf("Calling mmap of size: %zu\n", size);
#endif
    ptr = mmap(NULL, size, FT_PROT_RW, FT_MAP_DEF, -1, 0);
    if (ptr == MAP_FAILED)
        return (NULL);
    return (ptr);
}

void            *get_new_area(size_t initial_size)
{
    void        *ptr;
    size_t      alloc_size;

	if (initial_size <= TINY_BLOCK)
		alloc_size = get_alloc_size(TINY_BLOCK);
    else if (initial_size <= SMALL_BLOCK)
        alloc_size = get_alloc_size(SMALL_BLOCK);
    else
        alloc_size = initial_size + sizeof(struct s_block);
	alloc_size += sizeof(struct s_area);
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
        num_allocs = num_pages * page_size / block_size;
#ifdef DEBUG
        printf("\rnum_allocs: %d", num_allocs);
		fflush(stdout);
    }
	printf("\n");
#else
	}
#endif
    return (num_pages);
}

size_t          get_alloc_size(size_t size)
{
    size_t      alloc_size;
    size_t      page_size;
    unsigned    pages_num;

    page_size = getpagesize();
    pages_num = get_pages_amount(page_size, size + sizeof(struct s_block));
    alloc_size = pages_num * page_size;
#ifdef DEBUG 
    printf("num_pages: %u\npage_size: %d\n", pages_num, getpagesize());
#endif
    return (alloc_size);
}

t_area          find_area(t_area initial_area, size_t size)
{
    t_area      cur_area;
    t_type      type;

    if (size <= TINY_BLOCK)
        type = TINY;
    else if (size <= SMALL_BLOCK)
        type = SMALL;
    else
        type = LARGE;
    cur_area = initial_area;
    while (cur_area != NULL)
    {
        if (cur_area->type == type)
            return (cur_area);
        cur_area = cur_area->next_area;
    }
    return (cur_area);
}

t_block         init_block(t_area area, void *blk_ptr, size_t data_size)
{
    t_block     blk;

    blk = (t_block)blk_ptr;
    blk->area = area;
    blk->size = data_size;
    blk->data = blk_ptr + sizeof(struct s_block);
    blk->next = NULL;
    return (blk);
}

t_area          add_new_area(t_area area, size_t size)
{
    t_area      cur_area;
    t_area      next_area;

    if (area == NULL)
        return (NULL);
    cur_area = area;
    while (cur_area->next_area != NULL)
        cur_area = cur_area->next_area;
    cur_area->next_area = get_new_area(size);
    if (cur_area->next_area != NULL)
    {
        next_area = cur_area->next_area;
        next_area->prev_area = cur_area;
    }
    return (cur_area->next_area);
}

t_block         get_free_block(t_block blk, size_t size)
{
    t_block     cur_blk;

    cur_blk = blk;
    while (cur_blk != NULL)
    {
        if (cur_blk->free == true && cur_blk->size >= size)
            return (cur_blk);
        cur_blk = cur_blk->next;
    }
    return (NULL);
}

t_block         get_last_block(t_area area)
{
    t_block     last;

    last = (t_block)area->first_block;
    while (last->next)
        last = last->next;
    return (last);
}

bool            area_space_enough(t_area area, size_t size)
{
	t_block	    last_blk;
	void		*end_last_blk;
    void        *end_addr;
    void		*next_addr;

    last_blk = get_last_block(area);
	end_last_blk = (void *)last_blk + sizeof(struct s_block) + last_blk->size;
    end_addr = area->curr_area + area->size;
    next_addr = end_last_blk + sizeof(struct s_block) + size;
#ifdef DEBUG
//	printf("Size needed: %zu, area size=%zu\n ", size + sizeof(struct s_block), area->size);
#endif
    return (next_addr <= end_addr);
}

t_block         add_new_block(t_area area, size_t data_size)
{
    t_block     blk_ptr;
    t_block     last;

    last = get_last_block(area);
    blk_ptr = (void *)last + sizeof(struct s_block) + last->size;
    blk_ptr = init_block(area, blk_ptr, data_size);
    last->next = blk_ptr;
    return (blk_ptr);
}

t_block         get_block(t_block blk, t_area area, size_t size)
{
    t_area      cur_area;

    cur_area = area;
    while (blk == NULL)
    {
        cur_area = find_area(cur_area, size);
        if (cur_area == NULL)
        {
            cur_area = add_new_area(area, size);
            if (cur_area == NULL)
                return (NULL);
            blk = init_block(cur_area, cur_area->first_block, size);
            if (blk != NULL)
                break; //questionable
        }
        blk = get_free_block((t_block)cur_area->first_block, size);
        if (blk == NULL)
        {
            if (area_space_enough(cur_area, size))
                blk = add_new_block(cur_area, size);
            else
                cur_area = cur_area->next_area;
        }
    }
    return (blk);
}

void            *ft_malloc(size_t size)
{
    t_area      initial_area;
    t_block     blk;

    if (g_addr)
    {
        initial_area = (t_area)g_addr;
    }
    else
    {
        initial_area = get_new_area(size);
        if (initial_area == NULL)
            return (NULL);
        g_addr = (void *)initial_area;
    }
    blk = NULL;
    blk = get_block(blk, initial_area, size);
    if (blk == NULL)
        return (NULL);
    blk->free = false;
    //mark_block_used(blk);
    return (blk->data);
}

