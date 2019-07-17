#include "ft_malloc.h"

void		copy_data(t_block old, t_block new)
{
	ft_memcpy(new->data, old->data, old->size);
}

void		*ft_realloc(void *ptr, size_t size)
{
	t_area	initial;
	t_block	block;
	t_block	new_blk;

	DEBUG_PRINTF("<= old: %p, new size: %zu\n", ptr, size);
	if (ptr == NULL || g_addr == NULL)
		return (ft_malloc(size));
	block = (t_block)(ptr - sizeof(struct s_block));
	initial = (t_area)g_addr;
	new_blk = get_block(NULL, initial, size);
	if (new_blk == NULL)
		return (NULL);
	copy_data(block, new_blk);
	ft_free(block);
	DEBUG_PRINTF("==>> new: %p\n\n", new_blk->data);
	return (new_blk->data);
}
