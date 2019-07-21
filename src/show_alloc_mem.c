#include "ft_malloc.h"

static void		print_ptr(size_t ptr)
{
	char		hex[16];
	int			h;
	int			i;

	ft_putstr("0x");
	if (ptr == 0)
		ft_putchar('0');
	ft_bzero(hex, 16);
	i = 0;
	while (ptr)
	{
		h = ptr % 16;
		if (h < 10)
			hex[i] = h + '0';
		else
			hex[i] = h + 'a' - 10;
		ptr /= 16;
		i++;
	}
	while (i >= 0)
	{
		if (hex[i])
			ft_putchar(hex[i]);
		i--;
	}
}

static void		print_area(t_area area)
{
	const char	*types[] = {
		"TINY ",
		"SMALL",
		"LARGE"
	};

	ft_putstr(types[area->type]);
	ft_putstr(" : ");
	print_ptr((size_t)area->curr_area);
	ft_putendl("");
}

static void		print_block(t_block block, size_t *total)
{
	size_t		size;

	size = block->size;
	*total += size;
	print_ptr((size_t)block);
	ft_putstr(" - ");
	print_ptr((size_t)block + block->size);
	ft_putstr(" : ");
	ft_putnbr(size);
	ft_putendl(" bytes");
}

void			show_alloc_mem(void)
{
	t_area		area;
	t_block		block;
	size_t		total;

	total = 0;
	sort_areas((t_area *)&g_addr);
	area = (t_area)g_addr;
	while (area != NULL)
	{
		print_area(area);
		block = (t_block)(area->first_block);
		while (block != NULL)
		{
			if (block->free == false)
				print_block(block, &total);
			block = block->next;
		}
		area = area->next_area;
	}
	ft_putstr("Total : ");
	ft_putnbr(total);
	ft_putendl(" bytes");
}
