/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: juportie <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/22 10:22:36 by juportie          #+#    #+#             */
/*   Updated: 2025/06/16 11:11:08 by juportie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include <stdio.h>
#include "../../libft/src/libft.h"
#include "../minishell.h"
#include "../signals_utils/signals_utils.h"
#include "parsing.h"
#include "tree_build.h"
#include "lexer.h"

static t_token	*extract_token(
	char *line,
	int *pos,
	t_token *prev_token,
	t_error *error)
{
	t_token	*token;

	token = alloc_token(error);
	if (*error)
		return (NULL);
	if (prev_token != NULL && prev_token->type == heredoc)
		*error = extract_heredoc(token, line, pos);
	else if (is_quote(line[*pos]))
		*error = extract_quotes(token, line, pos);
	else if (is_operator(&line[*pos]))
		*error = extract_operator(token, line, pos);
	else if (is_expanding(line[*pos]))
		*error = extract_expanding(token, line, pos);
	else
		*error = extract_literal(token, line, pos);
	if (*error)
	{
		free(token->str);
		free(token);
		token = NULL;
		if (*error == critical)
			return (NULL);
	}
	return (token);
}

static t_error	incorrect_token_succession(
	t_dlst **tokens_list,
	t_token *token,
	t_error *error)
{
	enum e_token_type	last_type;
	enum e_token_type	new_type;

	if (!*tokens_list)
		return (success);
	last_type = get_toklist_type(ft_dlstlast(*tokens_list));
	new_type = token->type;
	if ((is_redir_op(new_type) && is_redir_op(last_type))
		|| ((is_control_op(new_type) || is_pipeline_op(new_type))
			&& (is_control_op(last_type) || is_pipeline_op(last_type))))
	{
		free(token->str);
		free(token);
		free_toklist(tokens_list);
		return (print_syntax_error(
				"unexpected token ", new_type, recoverable, error));
	}
	else
		return (success);
}

t_error	append_token_to_list(
	t_dlst **tokens_list,
	t_token *token,
	t_error *error)
{
	t_dlst	*new_node;

	if (token == NULL)
		return (*error);
	if (incorrect_token_succession(tokens_list, token, error))
		return (*error);
	new_node = ft_dlstnew(token);
	if (new_node == NULL)
	{
		free_token_content(token);
		free(token);
		free_toklist(tokens_list);
		return (err_print_alloc(critical, error));
	}
	ft_dlstadd_back(tokens_list, new_node);
	return (success);
}

t_dlst	*scan_line(char *line, t_error *error)
{
	int		pos;
	t_token	*token;
	t_dlst	*tokens_list;
	t_bool	cat_prev;

	pos = 0;
	token = NULL;
	tokens_list = NULL;
	while (1)
	{
		cat_prev = !skip_spaces(line, &pos);
		if (line[pos] == '\0')
			break ;
		token = extract_token(line, &pos, token, error);
		if (g_sig || !token)
		{
			free_toklist(&tokens_list);
			g_sig = 0;
			return (NULL);
		}
		token->cat_prev = cat_prev;
		if (append_token_to_list(&tokens_list, token, error) != success)
			return (NULL);
	}
	return (tokens_list);
}
