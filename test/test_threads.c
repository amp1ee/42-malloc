/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   test_threads.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oahieiev <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/09/28 18:07:32 by oahieiev          #+#    #+#             */
/*   Updated: 2019/09/28 18:07:33 by oahieiev         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/ft_malloc.h"
#include <string.h>
#include <pthread.h>
#include <unistd.h>

void			*thread_runner(void *vargp)
{
	char		*str;

	str = malloc(SMALL_BLOCK);
	if (!str)
		return (NULL);
	show_alloc_mem();
	str[0] = 42;
	str[1] = '\n';
	str[2] = 0;
	pthread_exit(str);
}

int				main(void)
{
	const int	num_threads = 16;
	pthread_t	thread_id[num_threads];
	int			i;
	char		*ret;

	i = -1;
	while (++i < num_threads)
		pthread_create(&thread_id[i], NULL, thread_runner, NULL);
	while (i-- > 0)
	{
		if ((pthread_join(thread_id[i], (void **)&ret)) != 0)
			return (-1);
		free(ret);
	}
	show_alloc_mem();
	return (0);
}
