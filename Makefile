NAME = ft_malloc

CFLAGS = -Wall -Wextra -Werror
ifdef DEBUG
    CFLAGS += -g
endif

SRC = $(addprefix src/, main.c)
INC = inc/

all:
	gcc $(CFLAGS) $(SRC) -I$(INC) -o $(NAME)
