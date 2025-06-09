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
SRC_BUILTINS_DIR := $(SRC_DIR)/builtins
SRC_CLEANING_UTILS_DIR := $(SRC_DIR)/cleaning_utils
SRC_ERROR_HANDLING_DIR := $(SRC_DIR)/error_handling
SRC_EXEC_DIR := $(SRC_DIR)/exec
SRC_EXPANSIONS_DIR := $(SRC_DIR)/expansions
SRC_GENERAL_UTILS_DIR := $(SRC_DIR)/general_utils
SRC_INPUT_DIR := $(SRC_DIR)/input
SRC_PARSING_DIR := $(SRC_DIR)/parsing
SRC_SHELL_INIT_DIR := $(SRC_DIR)/shell_init
SRC_SIGNALS_DIR := $(SRC_DIR)/signals_utils
vpath %.c $(SRC_DIR):\
	$(SRC_BUILTINS_DIR):\
	$(SRC_CLEANING_UTILS_DIR):\
	$(SRC_ERROR_HANDLING_DIR):\
	$(SRC_EXEC_DIR):\
	$(SRC_EXPANSIONS_DIR):\
	$(SRC_GENERAL_UTILS_DIR):\
	$(SRC_INPUT_DIR):\
	$(SRC_PARSING_DIR):\
	$(SRC_SHELL_INIT_DIR)\
	$(SRC_SIGNALS_DIR)
SRC_BUILTINS := cd.c \
					cd_cdpath_utils.c \
					cd_cwd_vars_utils.c \
					cd_utils.c \
					echo.c \
					env.c \
					export.c \
					export_errors_utils.c \
					export_utils.c \
					pwd.c \
					unset.c \
					exit.c
SRC_CLEANING_UTILS := cleaning_utils_1.c
SRC_ERROR_HANDLING := error_utils_1.c \
						error_utils_2.c
SRC_EXEC := exec_command.c \
				exec_clean_utils.c \
				exec_command_tree.c \
				exec_errors_utils.c \
				path_parsing.c \
				pipeline_handling.c \
				subprocess_utils.c \
				subshells_handling.c \
				redirections_utils.c \
				redirs_expansion_utils.c \
				builtins_handling.c \
				builtins_redir_utils.c

SRC_EXPANSIONS := expansions.c \
				  expansions_utils.c \
				  expansions_toklist_utils.c \
				  expansions_variable.c \
				  expansions_double_quotes.c \
				  expansions_wildcard.c \
				  heredoc_to_file.c
SRC_GENERAL_UTILS := env_utils.c \
					 general_utils.c \
					 paths_utils.c
SRC_INPUT := input.c \
			 input_loop_utils.c
SRC_SIGNALS := sigaction_init.c \
			   sigint_init.c \
			   sigquit_init.c \
			   signal_handlers.c
SRC_PARSING := lexer.c \
			   char_identity_check.c \
			   lexer_utils.c \
			   token_extract.c \
			   token_extract_utils.c \
			   tokens_list_get.c \
			   tokens_list_set.c \
			   print_utils.c \
			   tokens_list_print.c \
			   tokens_list_free.c \
			   tree_print.c \
			   tree_free.c \
			   tree_utils.c \
			   tree_build.c \
			   error_print.c \
			   parenthesis_handling_utils.c \
			   token_extract_grouping.c \
			   token_extract_heredoc.c \
			   token_extract_heredoc_delimiter.c \
			   token_type_identity_check.c \
			   tree_build_binary_op.c \
			   tree_build_redirections.c \
			   tree_build_grouping.c
SRC_SHELL_INIT := shell_init.c
SRC := minishell.c \
	   $(SRC_BUILTINS) \
	   $(SRC_CLEANING_UTILS) \
	   $(SRC_ERROR_HANDLING) \
	   $(SRC_EXEC) \
	   $(SRC_EXPANSIONS) \
	   $(SRC_GENERAL_UTILS) \
	   $(SRC_INPUT) \
	   $(SRC_PARSING) \
	   $(SRC_SHELL_INIT) \
	   $(SRC_SIGNALS)

BUILD_DIR := build
OBJ := $(addprefix $(BUILD_DIR)/, $(SRC:.c=.o))

DEP_FLAGS = -MMD
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
