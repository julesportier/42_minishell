/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: juportie <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/22 10:24:06 by juportie          #+#    #+#             */
/*   Updated: 2025/05/06 15:23:47 by juportie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PARSING_H
# define PARSING_H

# include "../../libft/src/libft.h"
# include "../minishell.h"

# define DEBUG 1

enum	e_token_type
{
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

// After expansions set redirections in t_block
// transform t_word into string array (char **) and put it into words member

// Structures to give to exec.
typedef struct	s_node_content
{
	//int is_subshell; // start from zero (no subshell) to N nested subshells
	// 01b heredoc, 00b standard file
	// 00b no expansions on input (if delimiter inside quotes or double quotes), 10b expands. Delimiter is never expanded.
	int	heredoc_flags;
	char	*input; // NULL if no redirection
	char	*output; // NULL if no redirection
	t_dlst	*tokens_list; // Tokens list
}	t_node_content;

typedef struct	s_bin_tree
{
    enum e_token_type	operator;
    struct s_bin_tree	*parent_node;
    struct s_bin_tree	*node_left;
    struct s_bin_tree	*node_right;
    t_node_content	*content;
}	t_bin_tree;

t_dlst	*scan_line(char *line, t_error *error);
// Tokens list utils (files: tokens_list_get.c, tokens_list_print.c)
enum e_token_type	get_toklist_type(t_dlst *list);
char	*get_toklist_str(t_dlst *list);
t_bool	get_toklist_cat_prev(t_dlst *list);
void	print_toklist(t_dlst *list);

#endif
