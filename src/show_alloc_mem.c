#include "ft_malloc.h"

#include <stdio.h> //disinclude
void			print_area(t_area area)
{
	static char *types[] = {
		"TINY",
		"SMALL",
		"LARGE"
	};

/*
	ft_putstr(types[area->type]);
	ft_putstr(" : ");
	ft_putnbr((int)area->curr_area);
	ft_putendl("");
*/
	printf("%s : %p\n", types[area->type], area->curr_area);
}

void			print_block(t_block block)
{
	size_t		size;

	size = block->size;
	printf("%p - %p : %zu bytes\n", (void *)block, (void *)(block + size), size);
}

void			show_alloc_mem(void)
{
	t_area		area;
	t_block		block;

	area = (t_area)g_addr;
	while (area != NULL)
	{
		print_area(area);
		block = (t_block)(area->first_block);
		while (block != NULL)
		{
			print_block(block);
			block = block->next;
		}
		area = area->next_area;
	}
}
