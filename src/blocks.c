#include "ft_malloc.h"

t_block			init_block(t_area area, void *blk_ptr, size_t datasize)
{
	t_block		blk;

	if (blk_ptr == NULL)
		return (NULL);
	blk = (t_block)blk_ptr;
	blk->area = area;
	blk->size = datasize;
	blk->data = blk_ptr + sizeof(struct s_block);
	blk->free = true;
	blk->next = NULL;
	return (blk);
}

t_block			get_last_block(t_area area)
{
	t_block		last;

	last = (t_block)area->first_block;
	while (last->next)
		last = last->next;
	return (last);
}

t_block			append_block(t_area area, size_t data_size)
{
	t_block		blk_ptr;
	t_block		last;

	last = get_last_block(area);
	blk_ptr = (void *)last + sizeof(struct s_block) + last->size;
	blk_ptr = init_block(area, blk_ptr, data_size);
	last->next = blk_ptr;
	return (blk_ptr);
}

t_block			get_free_block(t_area area, size_t size)
{
	t_block		cur_blk;

	cur_blk = (t_block)area->first_block;
	while (cur_blk != NULL)
	{
		if (cur_blk->free == true && cur_blk->size >= size)
		{
			return (init_block(area, cur_blk, size));
		}
		cur_blk = cur_blk->next;
	}
	if (area_space_enough(area, size))
		return (append_block(area, size));
	return (NULL);
}

t_block			get_block(t_area area, size_t size)
{
	t_area		cur_area;
	t_block		blk;

	cur_area = area;
	blk = NULL;
	while (blk == NULL)
	{
		cur_area = find_area(cur_area, size);
		if (cur_area == NULL)
		{
			cur_area = append_area(area, size);
			if (cur_area == NULL)
				return (NULL);
			blk = init_block(cur_area, cur_area->first_block, size);
			return (blk);
		}
		blk = get_free_block(cur_area, size);
		cur_area = cur_area->next_area;
	}
	return (blk);
}
