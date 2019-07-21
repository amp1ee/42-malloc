#include <stdio.h>
//#include <stdlib.h>
#include "../inc/ft_malloc.h"
#include <string.h>
#include <pthread.h>

#include <unistd.h>

void	print(char *str)
{
	write(1, str, strlen(str));
}

void *myThreadFun(void *vargp)
{
		char    *stro = NULL;
		char	*str;
		for(int    i = 0; i < 42; i++)
		{
			//str = malloc(16);
			str = ft_realloc(stro, i);
			show_alloc_mem();
			str[0] = 42;
			str[1] = '\n';
			str[2] = 0;
			print(str);
			//str = malloc(16);
			//free(str);
		}
		return (NULL);
}

int main()
{
		pthread_t thread_id[42];
		for(int i = 0; i < 42; i++)
		{
			//printf("pthread_create %d\n", i);
			pthread_create(&thread_id[i], NULL, myThreadFun, NULL);
		}
		for(int i = 0; i < 42; i++)
		{
			//printf("OK: %d\n", i);
			if ((pthread_join(thread_id[i], NULL)) != 0)
				return (-2);
		}
		return (0);
};
