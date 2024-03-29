/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oahieiev <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/08/02 15:38:19 by oahieiev          #+#    #+#             */
/*   Updated: 2019/08/02 15:38:21 by oahieiev         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_malloc.h"

void			unmap_area(t_area area)
{
	t_area		next;
	t_area		prev;
	t_block		blk;

	if (area != NULL)
	{
		next = (t_area)(area->next_area);
		prev = (t_area)(area->prev_area);
		blk = (t_block)(area->first_block);
		while (blk != NULL)
		{
			if (blk->free == false)
				return ;
			blk = blk->next;
		}
		munmap(area->curr_area, area->size);
		if (prev)
			prev->next_area = (void *)next;
		else
			g_addr = next;
		if (next)
			next->prev_area = (void *)prev;
	}
}

bool			verify_block(t_block blk)
{
	t_area		area;
	t_block		cur_blk;

	if (blk == NULL)
		return (false);
	area = (t_area)g_addr;
	while (area != NULL)
	{
		cur_blk = (t_block)(area->first_block);
		while (cur_blk)
		{
			if (cur_blk == blk)
				return (true);
			cur_blk = cur_blk->next;
		}
		area = area->next_area;
	}
	return (false);
}

void			free(void *ptr)
{
	t_block		blk;

	pthread_mutex_lock(&g_lock);
	if (g_addr != NULL && ptr != NULL)
	{
		blk = (t_block)(ptr - sizeof(struct s_block));
		if (verify_block(blk) == true)
		{
			if (blk->free != true)
			{
				blk->free = true;
				unmap_area((t_area)blk->area);
			}
		}
	}
	pthread_mutex_unlock(&g_lock);
}
