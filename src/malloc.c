#include "ft_malloc.h"

unsigned		get_pages_amount(size_t block_size, size_t page_size)
{
	unsigned	num_pages;
	unsigned	num_allocs;

	num_allocs = 0;
	num_pages = 0;

	if (block_size == 0 || page_size == 0)
		return (0);
	while (num_allocs < MIN_NUM_ALLOCS)
	{
		num_pages++;
		num_allocs = num_pages * page_size / block_size;
	}
	return (num_pages);
}

size_t			get_alloc_size(size_t size, size_t page_size)
{
	size_t		alloc_size;
	unsigned	pages_num;

	pages_num = get_pages_amount(size + sizeof(struct s_block), page_size);
	alloc_size = pages_num * page_size;
	return (alloc_size);
}

bool			area_space_enough(t_area area, size_t size)
{
	t_block		last_blk;
	void		*end_last_blk;
	void		*end_addr;
	void		*next_addr;

	last_blk = get_last_block(area);
	end_last_blk = (void *)last_blk + sizeof(struct s_block) + last_blk->size;
	end_addr = area->curr_area + area->size;
	next_addr = end_last_blk + sizeof(struct s_block) + size;
	return (next_addr <= end_addr);
}

void			*malloc(size_t size)
{
	t_area		initial_area;
	t_block		blk;

	if (size == 0)
		return (NULL);
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
	return (blk->data);
}
