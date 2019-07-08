NAME = ft_malloc

CFLAGS = -Wall -Wextra -Werror
SRC = $(addprefix src/, main.c)
INC = inc/

all:
	gcc $(CFLAGS) $(SRC) -I$(INC) -o $(NAME)
