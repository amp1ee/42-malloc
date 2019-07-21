#include "ft_malloc.h"

pthread_mutex_t	g_lock = PTHREAD_MUTEX_INITIALIZER;

size_t			align_size(size_t size, size_t page_size)
{
	if (page_size == 0 || size == 0)
		return (0);
	return ((((size - 1) / page_size) * page_size) + page_size);
}

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

void			*get_new_area(size_t initial_size)
{
	void		*ptr;
	size_t		alloc_size;
	size_t		page_size;

	page_size = getpagesize();
	if (initial_size <= TINY_BLOCK)
		alloc_size = get_alloc_size(TINY_BLOCK, page_size);
	else if (initial_size <= SMALL_BLOCK)
		alloc_size = get_alloc_size(SMALL_BLOCK, page_size);
	else
		alloc_size = initial_size + sizeof(struct s_block);
	alloc_size += sizeof(struct s_area);
	alloc_size = align_size(alloc_size, page_size);
	ptr = allocate_area(alloc_size);
	if (ptr != NULL)
		init_area(ptr, alloc_size, initial_size);
	return (ptr);
}

void			*unlock_and_return(void *ptr)
{
	pthread_mutex_unlock(&g_lock);
	return (ptr);
}

void			*malloc(size_t size)
{
	t_area		initial_area;
	t_block		blk;

	if (size == 0)
		return (NULL);
	pthread_mutex_lock(&g_lock);
	if (g_addr)
		initial_area = (t_area)g_addr;
	else
	{
		initial_area = get_new_area(size);
		if (initial_area == NULL)
			return (unlock_and_return(NULL));
		init_block(initial_area, initial_area->first_block, size);
		g_addr = (void *)initial_area;
	}
	blk = get_block(initial_area, size);
	if (blk == NULL)
		return (unlock_and_return(NULL));
	blk->free = false;
	return (unlock_and_return(blk->data));
}
