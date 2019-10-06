ifeq ($(HOSTTYPE),)
	HOSTTYPE := $(shell uname -m)_$(shell uname -s)
endif

SONAME = libft_malloc_$(HOSTTYPE).so
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

LDFLAGS := -lft -L$(LIBFT_D) -shared

TESTDIR = test/
TEST ?= $(TESTDIR)test_1.c
TESTOBJ = $(TEST:$(TESTDIR)%.c=$(TESTDIR)%.o)
TESTELF = $(notdir $(basename $(TEST)))

YELLW="\033[1;33m"
GREEN="\033[1;32m"
RESET="\033[0m"

.PHONY: all clean fclean re test

all: $(SONAME) $(LNAME)

$(SONAME): $(OBJ) $(LIBFT)
	@echo $(GREEN)"Linking "$(RESET)"$(SONAME)"
	@gcc $(OBJ) -o $(SONAME) $(LDFLAGS)

$(LNAME): $(SONAME)
	@echo "\t$(LNAME) -> $(SONAME)"
	@ln -sf $(SONAME) $(LNAME)
	@echo $(GREEN)"\nlibft_malloc is ready."
	@echo "To compile a test binary run:" $(YELLW) "\n\t make test TEST=test/[file].c" $(RESET)

$(OBJ_D)%.o: $(SRC_D)%.c
	@echo $(GREEN)"Compiling "$(RESET)"$<"
	@mkdir -p $(OBJ_D)
	@gcc $(CFLAGS) -c $< -o $@ $(INCLUDES)

$(LIBFT):
	@make -sC $(LIBFT_D)

test: $(TESTELF)

$(TESTELF): $(TESTOBJ) $(LNAME)
	@gcc $(TESTOBJ) -o $(TESTELF) -lft_malloc -L.

$(TESTDIR)%.o: $(TESTDIR)%.c
	@echo $(GREEN)"Compiling "$(RESET)"$(TEST)"
	@gcc -pthread $(CFLAGS) -c $< -o $@ $(INCLUDES)

clean:
	@echo $(YELLW)"Deleting object files..." $(RESET)
	@make -sC $(LIBFT_D) clean
	@rm -f $(TESTDIR)/*.o
	@rm -rf $(OBJ_D)

fclean: clean
	@echo $(YELLW)"Deleting binaries..." $(RESET)
	@rm -f $(SONAME) $(LNAME)
	@find . -name 'test*' -maxdepth 1 -type f -exec rm {} +
	@echo $(GREEN)"Cleanup done\n" $(RESET)

re: fclean all
