#ifndef FT_MALLOC_H
# define FT_MALLOC_H

# include <sys/types.h>
# include <unistd.h>
# include <sys/mman.h>
# include <stdbool.h>

# define ALIGN4(x) (((((x) - 1) >> 2) << 2) + 4)
# define FT_PROT_RW (PROT_READ | PROT_WRITE)
# define FT_MAP_DEF (MAP_PRIVATE | MAP_ANON)
# define MIN_NUM_ALLOCS (100)
# define TINY_BLOCK (1024)
# define SMALL_BLOCK (64 * TINY_BLOCK)

void                    *g_addr;

typedef int             t_bool;

typedef enum            e_blocktype
{
    TINY,
    SMALL,
    LARGE
}                       t_blocktype;

typedef struct          s_meta_blk
{
    size_t              size;
    t_blocktype         type;
    t_bool              free;
    struct s_meta_blk   *next;
    struct s_meta_blk   *prev;
    void                *data;
}                       *t_meta_blk;

void            *get_area(size_t size);
unsigned        get_pages_amount(size_t page_size, size_t block_size);
size_t          get_alloc_size(size_t size);
void            *ft_malloc(size_t size);

#endif
