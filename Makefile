NAME = ft_malloc

CFLAGS = -Wall -Wextra -Werror
ifdef DEBUG
    CFLAGS += -g -DDEBUG=1
endif

SRC = $(addprefix src/, malloc.c free.c)
MAIN ?= src/main.c
ifneq (,$(findstring libc, $(MAIN)))
	NAME = ft_malloc.libc
	CFLAGS += -O0
endif

INC = inc/

all:
	gcc $(CFLAGS) $(SRC) $(MAIN) -I$(INC) -o $(NAME)
