#include "ft_malloc.h"

void			print_ptr(size_t ptr)
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

t_area			find_min(t_area area) {
	t_area		cur;
	t_area		min;

	min = area;
	if (area != NULL)
	{
		cur = area->next_area;
		while (cur != NULL)
		{
			if ((size_t)min > (size_t)cur)
					min = cur;
			cur = cur->next_area;
		}
	}
	return (min);
}

void			swap_areas(t_area p1, t_area p2, t_area *start)
{
	t_area p1pre = NULL;
	t_area p1curr = *start;

	while (p1curr != p1)
	{
		p1pre = p1curr;
		p1curr = p1curr->next_area;
	}
	t_area p2pre = NULL;
	t_area p2curr = *start;
	while (p2curr != p2)
	{
		p2pre = p2curr;
		p2curr = p2curr->next_area;
	}
	if (p1pre != NULL)
	{
		p1pre->next_area = p2curr;
		p2curr->prev_area = p1pre;
	}
	else
	{
		*start = p2curr;
		(*start)->prev_area = NULL;
	}
	if (p2pre != NULL)
	{
		p2pre->next_area = p1curr;
		p1curr->prev_area = p2pre;
	}
	else
	{
		*start = p1curr;
		(*start)->prev_area = NULL;
	}

	t_area temp = p2curr->next_area;
	p2curr->next_area = p1curr->next_area;
	p1curr->next_area = temp;

	if (p2curr->next_area != NULL)
		((t_area)(p2curr->next_area))->prev_area = p2curr;
	if (p1curr->next_area != NULL)
		((t_area)(p1curr->next_area))->prev_area = p1curr;
}

void			sort_areas(t_area *start) {
	t_area		min;

	if ((*start)->next_area == NULL || *start == NULL)
		return ;
	min = find_min(*start);
	swap_areas(*start, min, start);
	sort_areas((t_area *)&((*start)->next_area));
}


void			print_area(t_area area)
{
	const char	*types[] = {
		"TINY",
		"SMALL",
		"LARGE"
	};

	ft_putstr(types[area->type]);
	ft_putstr(" : ");
	print_ptr((size_t)area->curr_area);
	ft_putendl("");
}

void			print_block(t_block block, size_t *total)
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
