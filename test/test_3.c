/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   test_3.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oahieiev <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/09/28 19:05:25 by oahieiev          #+#    #+#             */
/*   Updated: 2019/09/28 19:05:27 by oahieiev         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include "../inc/ft_malloc.h"

int			main(void)
{
	char	*tiny;

	tiny = calloc(2048, 1024);
	if (tiny == NULL)
		return (42);
	show_alloc_mem();
	free(tiny);
	ft_putstr("\nfreed\n");
	show_alloc_mem();
	return (0);
}
