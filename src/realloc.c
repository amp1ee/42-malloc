#include "ft_malloc.h"

void		copy_data(t_block old, t_block new)
{
	size_t	res_size;

	res_size = (old->size > new->size) ? new->size : old->size;
	ft_memcpy(new->data, old->data, res_size);
}

void		*ft_realloc(void *ptr, size_t size)
{
	t_block	block;
	t_block	new_blk;

	DEBUG_PRINTF("<= old: %p, new size: %zu\n", ptr, size);
	if (ptr == NULL || g_addr == NULL)
		return (ft_malloc(size));
	block = (t_block)(ptr - sizeof(struct s_block));
	if (size == 0)
	{
		ft_free(block->data);
		return (NULL);
	}
	new_blk = get_block(NULL, (t_area)g_addr, size);
	if (new_blk == NULL)
		return (NULL);
	copy_data(block, new_blk);
	new_blk->free = false;
	ft_free(block->data);
	DEBUG_PRINTF("==>> new: %p\n\n", new_blk->data);
	return (new_blk->data);
}