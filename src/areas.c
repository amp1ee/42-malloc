#include "ft_malloc.h"

void			init_area(void *ptr, size_t res_size, size_t initial_size)
{
	t_area		area;

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
}

void			*allocate_area(size_t size)
{
	void		*ptr;

	ptr = mmap(NULL, size, FT_PROT_RW, FT_MAP_DEF, -1, 0);
	if (ptr == MAP_FAILED)
		return (NULL);
	return (ptr);
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

t_area			find_area(t_area initial_area, size_t size)
{
	t_area		cur_area;
	t_type		type;

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

t_area			append_area(t_area area, size_t size)
{
	t_area		cur_area;
	t_area		next_area;

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
