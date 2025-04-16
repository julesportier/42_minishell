CC := cc
CFLAGS := -Wall -Werror -Wextra
CFLAGS_DB := -Wall -Werror -Wextra -g3
CFLAGS_NE := -g3
CFLAGS_REL := -Wall -Werror -Wextra -O2

LIBFT_DIR := ./libft
LIBFT_AR := $(LIBFT_DIR)/libft.a
LIBFT_HEADERS := $(LIBFT_DIR)/src/libft.h \
		 $(LIBFT_DIR)/src/ft_printf.h \
		 $(LIBFT_DIR)/src/get_next_line.h

LIBS_FLAGS := -lft -L$(LIBFT_DIR) -lreadline

NAME := minishell

SRC_DIR := src
vpath %.c $(SRC_DIR)
# Replace the 2 lines above by sources files names for release
SRC := $(wildcard $(SRC_DIR)/*.c)
SRC := $(patsubst $(SRC_DIR)/%.c, %.c, $(SRC))

BUILD_DIR := build
OBJ := $(addprefix $(BUILD_DIR)/, $(SRC:.c=.o))

DEP_FLAGS = -MD
DEP_FILES := $(OBJ:.o=.d)

all: $(BUILD_DIR) ft $(NAME)

noerr: CFLAGS = $(CFLAGS_NE)
noerr: all
dbg: CFLAGS = $(CFLAGS_DB)
dbg: all
rel: CFLAGS = $(CFLAGS_REL)
rel: all

ft:
	@git submodule init
	@git submodule update
	make CFLAGS="$(CFLAGS)" -C $(LIBFT_DIR)

-include $(DEP_FILES)

$(NAME): $(OBJ) $(LIBFT_AR)
	$(CC) $^ -o $@ $(LIBS_FLAGS)

$(BUILD_DIR)/%.o: %.c Makefile
	$(CC) $(CFLAGS) $(DEP_FLAGS) -c $< -o $@

$(BUILD_DIR):
	mkdir -p $@

clean:
	make clean -C $(LIBFT_DIR)
	rm -rf $(BUILD_DIR)

fclean:
	make fclean -C $(LIBFT_DIR)
	rm -rf $(BUILD_DIR)
	rm -f $(NAME)

re: fclean all

.PHONY: all ft clean fclean re noerr dbg rel
