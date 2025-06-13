/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: juportie <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/22 10:24:06 by juportie          #+#    #+#             */
/*   Updated: 2025/05/26 14:43:04 by juportie         ###   ########.fr       */
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
	heredoc,
	heredoc_exp,
	heredoc_lit
};

typedef struct s_token
{
	t_bool				cat_prev;
	enum e_token_type	type;
	char				*str;
}	t_token;

typedef struct s_node_content
{
	t_bool	subshell;
	t_dlst	*inputs;
	t_dlst	*outputs;
	t_dlst	*tokens_list;
}	t_node_content;

typedef struct s_bin_tree
{
	enum e_token_type	operator;
	struct s_bin_tree	*parent;
	struct s_bin_tree	*left;
	struct s_bin_tree	*right;
	t_node_content		*content;
}	t_bin_tree;

/**********
 * LEXING *
 * ********/
// lexer.c
t_error				append_token_to_list(
						t_dlst **tokens_list,
						t_token *token,
						t_error *error);
t_dlst				*scan_line(char *line, t_error *error);

/***********
 * PARSING *
 * *********/
// tree_build.c
t_bin_tree			*build_parse_tree(t_dlst **toklist, t_error *error);

/********
 * FREE *
 * ******/
// tokens_list_free.c
void				free_token_content(void *content);
void				free_toklist(t_dlst **toklist);
// tree_free.c
void				free_tree(t_bin_tree **tree);

/******************
 * ERROR HANDLING *
 * ****************/
// error_print.c
t_error				print_syntax_error(
						char *message,
						enum e_token_type type,
						t_error errnum,
						t_error *error);
t_error				err_perror_alloc(t_error errnum, t_error *error);
void				*null_perror_alloc(t_error errnum, t_error *error);
t_error				err_print_alloc(t_error errnum, t_error *error);
void				*null_print_alloc(t_error errnum, t_error *error);
/************
 * PRINTING *
 * **********/
// print_utils.c
void				print_indent(int indent_level);
const char			*bool_to_str(t_bool bool);
// tokens_list_print.c
const char			*token_type_to_str(enum e_token_type type);
void				print_toklist(t_dlst *list, int indent);
// tree_print.c
void				print_tree(t_bin_tree *tree, int level);

/*********
 * UTILS *
 * *******/
// tokens_list_get.c
enum e_token_type	get_toklist_type(t_dlst *list);
char				*get_toklist_str(t_dlst *list);
t_bool				get_toklist_cat_prev(t_dlst *list);
// tokens_list_set.c
void				set_toklist_type(t_dlst *list, enum e_token_type type);
void				set_toklist_str(t_dlst *list, char *str);
void				set_toklist_cat_prev(t_dlst *list, t_bool cat_prev);
// tree_utils.c
t_bin_tree			*tree_root(t_bin_tree *node);
t_bin_tree			*alloc_tree_node(t_error *error);
// parenthesis_handling_utils.c
int					update_parenthesis_nesting_level(
						t_dlst *token,
						int nesting_level);
t_error				check_nesting_level(int nesting_level, t_dlst *toklist);

#endif
