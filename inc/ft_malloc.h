#ifndef FT_MALLOC_H
# define FT_MALLOC_H

# include <sys/types.h>
# include <unistd.h>
# include <sys/mman.h>
# include <stdbool.h>

#ifdef DEBUG
# include <stdio.h>
#endif

# define ALIGN4(x) (((((x) - 1) >> 2) << 2) + 4)
# define FT_PROT_RW (PROT_READ | PROT_WRITE)
# define FT_MAP_DEF (MAP_PRIVATE | MAP_ANON)
# define MIN_NUM_ALLOCS (200)
# define TINY_BLOCK (1024)
# define SMALL_BLOCK (64 * TINY_BLOCK)

void                    *g_addr;

//typedef int             t_bool;

typedef enum            e_type
{
    TINY,
    SMALL,
    LARGE
}                       t_type;

typedef struct          s_block
{
    void                *area;
    size_t              size;
    struct s_block      *next;
    void                *data;
    bool                free;
}                       *t_block;

typedef struct          s_area
{
    t_block             *first_block;
    size_t              size;
    void                *curr_area;
    void                *next_area;
    void                *prev_area;
	t_type				type;
}                       *t_area;

void            *get_area(size_t size);
unsigned        get_pages_amount(size_t page_size, size_t block_size);
size_t          get_alloc_size(size_t size);
void            ft_free(void *ptr);
void            *ft_malloc(size_t size);

#endif
