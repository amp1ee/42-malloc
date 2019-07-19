#ifndef FT_MALLOC_H
# define FT_MALLOC_H

# include <sys/types.h>
# include <unistd.h>
# include <sys/mman.h>
# include <stdbool.h>
# include "libft.h"

#ifdef DEBUG
# include <stdio.h>
# include <stdarg.h>
void	dbg_printf(const char *func, const char *format, ...);
# define DEBUG_PRINTF(...) dbg_printf(__func__, __VA_ARGS__)
#else
# define DEBUG_PRINTF(...)
#endif

# define ALIGN4(x)		(((((x) - 1) >> 2) << 2) + 4)
# define FT_PROT_RW		(PROT_READ | PROT_WRITE)
# define FT_MAP_DEF		(MAP_PRIVATE | MAP_ANON)
# define MIN_NUM_ALLOCS (100)
# define TINY_BLOCK		(1024)
# define SMALL_BLOCK	(64 * TINY_BLOCK)

void					*g_addr;

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
}						*t_block;

typedef struct			s_area
{
	size_t				size;
	void				*curr_area;
	void				*next_area;
	void				*prev_area;
	t_type				type;
	void				*first_block;
}						*t_area;

void					*get_area(size_t size);
unsigned				get_pages_amount(size_t page_size, size_t block_size);
size_t					get_alloc_size(size_t size);
void					ft_free(void *ptr);
void					*ft_malloc(size_t size);
void					show_alloc_mem(void);
void					*ft_realloc(void *ptr, size_t size);
void					*ft_memcpy(void *dst, const void *src, size_t n);
t_block					get_block(t_block blk, t_area area, size_t size);
bool					verify_block(t_block blk);

#endif
