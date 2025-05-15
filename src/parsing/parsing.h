/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ecasalin <ecasalin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/22 10:24:06 by juportie          #+#    #+#             */
/*   Updated: 2025/05/15 09:01:41 by ecasalin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PARSING_H
# define PARSING_H

# include "../../libft/src/libft.h"
# include "../minishell.h"

# define DEBUG 1

enum	e_token_type
{
	null,
	literal,
	double_quotes,
	variable,
	wildcard,
	or,
	and,
	pipeline,
	left_parenthesis,
	right_parenthesis,
	redir_output,
	append_output,
	redir_input,
	heredoc
};

typedef struct	s_token
{
	// To know if the word will be separated by spaces. `echo $HOME"text"1` is
	// four tokens but expands to two words `echo /home/juportietext1`.
	t_bool	cat_prev;
	enum e_token_type	type;
	char	*str;
}	t_token;

// Structures to give to exec.
typedef struct	s_node_content
{
	//int is_subshell; // start from zero (no subshell) to N nested subshells
	// 01b heredoc, 00b standard file
	// 00b no expansions on input (if delimiter inside quotes or double quotes), 10b expands. Delimiter is never expanded.
	int	heredoc_flags;
	t_dlst	*inputs; // NULL if no redirection
	t_dlst	*outputs; // NULL if no redirection
	t_dlst	*tokens_list; // Tokens list
}	t_node_content;

typedef struct	s_bin_tree
{
    enum e_token_type	operator;
    struct s_bin_tree	*parent;
    struct s_bin_tree	*left;
    struct s_bin_tree	*right;
    t_node_content	*content;
}	t_bin_tree;

/**********
 * LEXING *
 * ********/
// lexer.c
t_dlst	*scan_line(char *line, t_error *error);

/********
 * FREE *
 * ******/
// tokens_list_free.c
void	free_token_content(void *content);
void	free_toklist(t_dlst **toklist);
// tree_free.c
void	free_tree(t_bin_tree *tree);

/******************
 * ERROR HANDLING *
 * ****************/
// error_print.c
int	print_syntax_error(char *message, enum e_token_type type, int errnum);

/************
 * PRINTING *
 * **********/
// print_utils.c
void	print_indent(int indent_level);
// tokens_list_print.c
const char	*token_type_to_str(enum e_token_type type);
void	print_toklist(t_dlst *list, int indent);
// tree_print.c
void	print_tree(t_bin_tree *tree, int level);

/*********
 * UTILS *
 * *******/
// tokens_list_get.c
enum e_token_type	get_toklist_type(t_dlst *list);
char	*get_toklist_str(t_dlst *list);
t_bool	get_toklist_cat_prev(t_dlst *list);
// tree_utils.c
t_bin_tree	*tree_root(t_bin_tree *node);
// parenthesis_handling_utils.c
int	update_parenthesis_nesting_level(t_dlst *token, int nesting_level);
int	check_nesting_level(int nesting_level, t_dlst *toklist);

#endif
