#include "ft_malloc.h"

static t_area	find_min(t_area area) 
{
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

static void		swap_areas_finish(t_area p1, t_area p2)
{
	t_area temp;

	temp = p2->next_area;
	p2->next_area = p1->next_area;
	p1->next_area = temp;
	if (p2->next_area != NULL)
		((t_area)(p2->next_area))->prev_area = p2;
	if (p1->next_area != NULL)
		((t_area)(p1->next_area))->prev_area = p1;
}

static void		swap_areas(t_area p1, t_area p2, t_area *start)
{
	t_area p1pre;
	t_area p2pre;

	p1pre = p1->prev_area;
	p2pre = p2->prev_area;
	if (p1pre != NULL)
	{
		p1pre->next_area = p2;
		p2->prev_area = p1pre;
	}
	else
		*start = p2;
	if (p2pre != NULL)
	{
		p2pre->next_area = p1;
		p1->prev_area = p2pre;
	}
	else
		*start = p1;
	if (p1pre == NULL || p2pre == NULL)
		(*start)->prev_area = NULL;
	swap_areas_finish(p1, p2);
}

void			sort_areas(t_area *start)
{
	t_area		min;

	if (*start == NULL || (*start)->next_area == NULL)
		return ;
	min = find_min(*start);
	swap_areas(*start, min, start);
	sort_areas((t_area *)&((*start)->next_area));
}