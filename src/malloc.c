#include "ft_malloc.h"

#ifdef DEBUG
# include <string.h>
void    dbg_printf(const char *func, const char *format, ...)
{
    va_list     va;
    size_t      max_size = 1024;
    char        msg[max_size];
	char		*log_fmt;

    va_start(va, format);
    vsnprintf(msg, max_size, format, va);
    va_end(va);
	log_fmt = (strlen(func) + 3 >= 16) ? "%s():\t%s" : "%s():\t\t%s";
    printf(log_fmt, func, msg);
}
#endif

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
    area->first_block = ptr + sizeof(struct s_area);
    area->curr_area = ptr;
    area->prev_area = NULL;
    area->next_area = NULL;
    area->size = res_size;
    DEBUG_PRINTF("area: %p, type: %d, 1st_blk: %p, area_size: %zu\n", ptr, area->type, area->first_block, area->size);
}

void            *allocate_area(size_t size)
{
    void        *ptr;

    DEBUG_PRINTF("Calling mmap of size: %zu\n", size);
    ptr = mmap(NULL, size, FT_PROT_RW, FT_MAP_DEF, -1, 0);
    DEBUG_PRINTF("Got addr: %p\n", ptr);
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
    DEBUG_PRINTF("initial_size: %zu\n", initial_size);
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
    }
    DEBUG_PRINTF("num_allocs: %d\n", num_allocs);
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
    DEBUG_PRINTF("num_pages: %u\n", pages_num);
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
        if (cur_area->type == type && cur_area->size > size)
            return (cur_area);
        cur_area = cur_area->next_area;
    }
    return (cur_area);
}

t_block         init_block(t_area area, void *blk_ptr, size_t data_size)
{
    t_block     blk;

    if (blk_ptr == NULL)
        return (NULL);
    blk = (t_block)blk_ptr;
    blk->area = area;
    blk->size = data_size;
    blk->data = blk_ptr + sizeof(struct s_block);
    blk->free = true;
    blk->next = NULL;
    DEBUG_PRINTF("area: %p, blk_ptr: %p, size: %zu, data: %p\n", area->curr_area, blk_ptr, blk->size, blk->data);
    return (blk);
}

t_area          append_area(t_area area, size_t size)
{
    t_area      cur_area;
    t_area      next_area;

    DEBUG_PRINTF("<- prev area: %p\n", area);
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
    DEBUG_PRINTF("-> new area: %p\n", cur_area->next_area);
    return (cur_area->next_area);
}

t_block         get_free_block(t_block blk, size_t size)
{
    t_block     cur_blk;

    cur_blk = blk;
    while (cur_blk != NULL)
    {
        if (cur_blk->free == true && cur_blk->size >= size)
        {
            DEBUG_PRINTF("free block: %p\n", (void *)cur_blk);
            return (cur_blk);
        }
        cur_blk = cur_blk->next;
    }
    DEBUG_PRINTF("no free block\n");
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
    DEBUG_PRINTF("Size needed: %zu, area size=%zu\n", size + sizeof(struct s_block), area->size);
    return (next_addr <= end_addr);
}

t_block         append_block(t_area area, size_t data_size)
{
    t_block     blk_ptr;
    t_block     last;

    last = get_last_block(area);
    blk_ptr = (void *)last + sizeof(struct s_block) + last->size;
    blk_ptr = init_block(area, blk_ptr, data_size);
    last->next = blk_ptr;
    DEBUG_PRINTF("area: %p, data_size: %zu, last_blk: %p, new_blk: %p, last->size: %zu\n", (void*)area, data_size, (void *)last, blk_ptr, last->size);
    return (blk_ptr);
}

t_block         get_block(t_block blk, t_area area, size_t size)
{
    t_area      cur_area;

    cur_area = area;
    while (blk == NULL)
    {
        cur_area = find_area(cur_area, size);
        DEBUG_PRINTF("find_area returned: %p\n", cur_area);
        if (cur_area == NULL)
        {
            cur_area = append_area(area, size);
            if (cur_area == NULL)
                return (NULL);
            blk = init_block(cur_area, cur_area->first_block, size);
            return (blk);
        }
        blk = get_free_block((t_block)cur_area->first_block, size);
        if (blk == NULL)
        {
            if (area_space_enough(cur_area, size))
                blk = append_block(cur_area, size);
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
        initial_area = (t_area)g_addr;
    else
    {
        initial_area = get_new_area(size);
        if (initial_area == NULL)
            return (NULL);
        init_block(initial_area, initial_area->first_block, size);
        g_addr = (void *)initial_area;
    }
    blk = NULL;
    blk = get_block(blk, initial_area, size);
    if (blk == NULL)
        return (NULL);
    blk->free = false;
	DEBUG_PRINTF("==>> Returning data ptr: %p\n\n", blk->data);
    return (blk->data);
}

