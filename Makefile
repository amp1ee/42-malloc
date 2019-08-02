ifeq ($(HOSTTYPE),)
	HOSTTYPE := $(shell uname -m)_$(shell uname -s)
endif

NAME = libft_malloc_$(HOSTTYPE).so
LNAME = libft_malloc.so

SRC_D := src/
SRC = $(addprefix $(SRC_D), malloc.c \
						free.c \
						realloc.c \
						calloc.c \
						areas.c \
						blocks.c \
						sort_areas.c \
						show_alloc_mem.c)

LIBFT_D := libft/
LIBFT := $(LIBFT_D)libft.a

OBJ_D := obj/
OBJ := $(SRC:$(SRC_D)%.c=$(OBJ_D)%.o)

INC_D := inc/

INCLUDES := -I$(INC_D) -I$(LIBFT_D)

CFLAGS := -Wall -Wextra -Werror -fPIC -pthread
ifdef DEBUG
	CFLAGS += -g
endif

LDFLAGS := -lft -L$(LIBFT_D) -shared -pthread
ifneq (, $(findstring Darwin, $(HOSTTYPE)))
	LDFLAGS += -Wl,-install_name,$(LNAME)
else
	LDFLAGS += -Wl,-soname,$(LNAME)
endif

MAIN ?= test/test001.c

.PHONY: all clean fclean re test

all: $(NAME) $(LNAME)

test: $(NAME)
	@gcc -g -pthread $(INCLUDES) $(MAIN) -o $(basename $(MAIN)) -lft_malloc -L.
	@echo "Run using this command\nDYLD_LIBRARY_PATH=`pwd` $(basename $(MAIN))"

$(NAME): $(OBJ) $(LIBFT)
	@echo "Linking $(NAME)"
	@gcc $(OBJ) -o $(NAME) $(LDFLAGS)

$(LNAME): $(NAME)
	@echo "$(LNAME) -> $(NAME)"
	@ln -sf $(NAME) $(LNAME)

$(OBJ_D)%.o: $(SRC_D)%.c
	@echo "Compiling $<"
	@mkdir -p $(OBJ_D)
	@gcc $(CFLAGS) -c $< -o $@ $(INCLUDES)

$(LIBFT):
	@make -sC $(LIBFT_D)

clean:
	@make -sC $(LIBFT_D) clean
	@rm -rf $(OBJ_D)

fclean: clean
	@make -sC $(LIBFT_D) fclean
	@rm -f $(NAME) $(LNAME)

re: fclean all
