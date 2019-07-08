#ifndef FT_MALLOC_H
# define FT_MALLOC_H

# include <sys/types.h>
# include <unistd.h>
# include <sys/mman.h>

# define ALIGN4(x) (((((x) - 1) >> 2) << 2) + 4)
# define FT_PROT_RW (PROT_READ | PROT_WRITE)
# define FT_MAP_DEF (MAP_PRIVATE | MAP_ANONYMOUS)
# define MALLOC_SIZE (getpagesize())

typedef int             t_bool;

typedef struct          s_meta_blk
{
    size_t              size;
    t_bool              free;
    struct s_meta_blk   *next;
}                       *t_meta_blk;

#endif
