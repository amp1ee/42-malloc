#include "ft_malloc.h"

void			unmap_area(t_area area)
{
	t_area		next;
	t_area		prev;
	t_block		blk;

	if (area != NULL)
	{
		next = area->next_area;
		prev = area->prev_area;
		blk = (t_block)area->first_block;
		while (blk != NULL)
		{
			if (blk->free == false)
				return ;
			blk = blk->next;
		}
		if (prev)
		{
			DEBUG_PRINTF("Calling munmap for area %p\n", (void *)area);
			munmap(area->curr_area, area->size);
			prev->next_area = next;
			if (next)
				next->prev_area = prev;
		}
	}
}

bool            verify_block(t_block blk)
{
    t_area      area;
    t_block     cur_blk;

    if (blk == NULL)
        return (false);
    area = (t_area)g_addr;
    while (area != NULL)
    {
        cur_blk = (t_block)area->first_block;
        while (cur_blk)
        {
            if (cur_blk == blk)
                return (true);
            cur_blk = cur_blk->next;
        }
        area = area->next_area;
    }
    return (false);
}

void            ft_free(void *ptr)
{
    t_block     blk;

    if (g_addr == NULL || ptr == NULL)
        return ;
    blk = (t_block)(ptr - sizeof(struct s_block));
    if (verify_block(blk) == true)
    {
        if (blk->free == true)
            return ;
        else
	{
            DEBUG_PRINTF("Freeing blk addr: %p\n", ptr);
            blk->free = true;
	    unmap_area((t_area)blk->area);
        }
    }
}
