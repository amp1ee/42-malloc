/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   realloc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oahieiev <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/08/02 15:37:29 by oahieiev          #+#    #+#             */
/*   Updated: 2019/08/02 15:39:00 by oahieiev         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_malloc.h"

void		*unlock_and_return(void *ptr)
{
	pthread_mutex_unlock(&g_lock);
	return (ptr);
}

void		*unlock_and_malloc(size_t size)
{
	pthread_mutex_unlock(&g_lock);
	return (malloc(size));
}

void		copy_data(t_block old, t_block new)
{
	size_t	res_size;

	res_size = (old->size > new->size) ? new->size : old->size;
	ft_memcpy(new->data, old->data, res_size);
}

void		*realloc(void *ptr, size_t size)
{
	t_block	block;
	t_block	new_blk;

	pthread_mutex_lock(&g_lock);
	if (ptr == NULL || g_addr == NULL)
		return (unlock_and_malloc(size));
	block = (t_block)(ptr - sizeof(struct s_block));
	if (verify_block(block) == false)
		return (unlock_and_malloc(size));
	if (size == 0)
	{
		pthread_mutex_unlock(&g_lock);
		free(block->data);
		return (NULL);
	}
	new_blk = get_block((t_area)g_addr, size);
	if (new_blk == NULL)
		return (unlock_and_return(NULL));
	copy_data(block, new_blk);
	new_blk->free = false;
	pthread_mutex_unlock(&g_lock);
	free(block->data);
	return (new_blk->data);
}
