#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>

void *myThreadFun(void *vargp)
{
		char    *str;
		for(int    i = 0; i < 10; i++)
		{
				str = malloc(16);
/*             str = malloc(16);
			   str = realloc(str, 16);
			   free(str);*/
		}
		return   NULL;
}

int main()
{
		pthread_t thread_id[42];
		for(int i = 0; i < 42; i++)
		{
			printf("pthread_create %d\n", i);
				pthread_create(&thread_id[i], NULL, myThreadFun, NULL);
				
		}
		for(int i = 0; i < 42; i++)
		{
				printf("OK: %d\n", i);
				if ((pthread_join(thread_id[i], NULL)) != 0)
					return (-2);
		}
		exit(0);
};
