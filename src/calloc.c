#include "ft_malloc.h"

void		*ft_calloc(size_t count, size_t size)
{
	void	*ptr;

	ptr = ft_malloc(count * size);
	pthread_mutex_lock(&g_lock);
	if (ptr != NULL)
		ft_bzero(ptr, count * size);
	pthread_mutex_unlock(&g_lock);
	return (ptr);
}