#include "../inc/ft_malloc.h"
#include <string.h>
#include <pthread.h>
#include <unistd.h>

void	print(char *str)
{
	if (str == NULL)
		return ;
	write(1, str, strlen(str));
}

void *thread_runner(void *vargp)
{
	char	*str;
	for (int i = 0; i < 10; i++)
	{
		str = malloc(16 + i);
		if (!str)
			return (NULL);
		show_alloc_mem();
		str[0] = 42;
		str[1] = '\n';
		str[2] = 0;
	}
	pthread_exit(str);
}

int main()
{
	int		T = 21;
	pthread_t thread_id[T];
	for (int i = 0; i < T; i++)
	{
		pthread_create(&thread_id[i], NULL, thread_runner, NULL);
	}
	for (int i = 0; i < T; i++)
	{
		char *ret;
		if ((pthread_join(thread_id[i], (void **)&ret)) != 0)
			return (-2);
		free(ret);
	}
	return (0);
};
