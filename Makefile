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

TEST ?= test/test001.c
elf = $(notdir $(basename $(1)))

YELLW="\033[1;33m"
GREEN="\033[1;32m"
RESET="\033[0m"

.PHONY: all clean fclean re test

all: $(NAME) $(LNAME)

test: $(NAME)
	@echo "Compiling $(TEST)"
	@gcc -pthread $(INCLUDES) $(TEST) -o $(call elf, $(TEST)) -lft_malloc -L.
	@echo "#Run using this command:\vDYLD_LIBRARY_PATH=`pwd` ./$(call elf, $(TEST))"

$(NAME): $(OBJ) $(LIBFT)
	@echo "Linking $(NAME)"
	@gcc $(OBJ) -o $(NAME) $(LDFLAGS)

$(LNAME): $(NAME)
	@echo "$(LNAME) -> $(NAME)"
	@ln -sf $(NAME) $(LNAME)
	@echo $(GREEN)"\nlibft_malloc is ready."
	@echo "To compile a test binary run:" $(YELLW) "\n\t make test TEST=test/[file].c" $(RESET)

$(OBJ_D)%.o: $(SRC_D)%.c
	@echo "Compiling $<"
	@mkdir -p $(OBJ_D)
	@gcc $(CFLAGS) -c $< -o $@ $(INCLUDES)

$(LIBFT):
	@make -sC $(LIBFT_D)

clean:
	@echo $(YELLW)"Deleting libft objects..." $(RESET)
	@make -sC $(LIBFT_D) clean
	@rm -rf $(OBJ_D)

fclean: clean
	@echo $(YELLW)"Deleting executables..." $(RESET)
	@rm -f $(NAME) $(LNAME)
	@find . -name 'test*' -type f -exec echo {} +
	@echo $(GREEN)"Cleanup done\n" $(RESET)

re: fclean all
