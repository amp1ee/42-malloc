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

void *myThreadFun(void *vargp)
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
	pthread_t thread_id[6];
	for (int i = 0; i < 6; i++)
	{
		pthread_create(&thread_id[i], NULL, myThreadFun, NULL);
	}
	for (int i = 0; i < 6; i++)
	{
		char *ret;
		if ((pthread_join(thread_id[i], (void **)&ret)) != 0)
			return (-2);
		print(ret);
		free(ret);
	}
	return (0);
};
