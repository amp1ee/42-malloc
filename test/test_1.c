/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   test_1.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oahieiev <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/09/28 18:44:31 by oahieiev          #+#    #+#             */
/*   Updated: 2019/09/28 18:44:38 by oahieiev         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>
#include <stdlib.h>
#include "../inc/ft_malloc.h"
#include <time.h>

#define TINY	(TINY_BLOCK)
#define SMALL	(SMALL_BLOCK)
#define LARGE	(8 * SMALL)
#define Q		(10)

int				main(void)
{
	size_t		i;
	size_t		j;
	char		*dptr[Q];
	int			r;

	srand(time(NULL));
	i = -1;
	while (++i < Q)
	{
		r = rand();
		dptr[i] = (char *)malloc(r % LARGE);
		if (dptr[i] == NULL)
			return (1);
		j = -1;
		while (++j < (r % LARGE))
			dptr[i][j] = (char)(j % (128 - 34) + 33);
	}
	show_alloc_mem();
	i = 0;
	while (i < Q)
		free(dptr[i++]);
	show_alloc_mem();
	return (0);
}
