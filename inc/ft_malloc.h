/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_malloc.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oahieiev <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/08/02 15:49:05 by oahieiev          #+#    #+#             */
/*   Updated: 2019/08/02 15:49:07 by oahieiev         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FT_MALLOC_H
# define FT_MALLOC_H

# include <sys/types.h>
# include <unistd.h>
# include <sys/mman.h>
# include <stdbool.h>
# include <pthread.h>
# include "libft.h"

# define FT_PROT_RW		(PROT_READ | PROT_WRITE)
# define FT_MAP_DEF		(MAP_PRIVATE | MAP_ANON)
# define MIN_NUM_ALLOCS (128)
# define TINY_BLOCK		(512)
# define SMALL_BLOCK	(4096)

void					*g_addr;
extern pthread_mutex_t	g_lock;

typedef enum			e_type
{
	TINY,
	SMALL,
	LARGE
}						t_type;

typedef struct			s_block
{
	void				*area;
	size_t				size;
	struct s_block		*next;
	void				*data;
	bool				free;
}					*	t_block;

typedef struct			s_area
{
	void				*curr_area;
	void				*next_area;
	void				*prev_area;
	size_t				size;
	t_type				type;
	void				*first_block;
}					*	t_area;

void					*malloc(size_t size);
void					*realloc(void *ptr, size_t size);
void					*calloc(size_t count, size_t size);
void					free(void *ptr);

unsigned				get_pages_amount(size_t block_size, size_t page_size);
size_t					align_size(size_t size, size_t page_size);
size_t					get_alloc_size(size_t size, size_t page_size);
void					show_alloc_mem(void);
void					sort_areas(t_area *start);

bool					verify_block(t_block blk);
t_block					init_block(t_area area, void *blk_ptr, size_t datasize);
t_block					get_free_block(t_area area, size_t size);
t_block					get_last_block(t_area area);
t_block					append_block(t_area area, size_t data_size);
t_block					get_block(t_area area, size_t size);
void					*unlock_and_return(void *ptr);

void					init_area(void *ptr, size_t res_size,
									size_t initial_size);
void					*allocate_area(size_t size);
void					*get_new_area(size_t initial_size);
t_area					find_area(t_area initial_area, size_t size);
t_area					append_area(t_area area, size_t size);
bool					area_space_enough(t_area area, size_t size);

#endif
