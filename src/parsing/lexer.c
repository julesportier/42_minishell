/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: juportie <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/22 10:22:36 by juportie          #+#    #+#             */
/*   Updated: 2025/05/14 12:56:24 by juportie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include <stdio.h>
#include "../../libft/src/libft.h"
#include "../minishell.h"
#include "parsing.h"
#include "lexer.h"

static t_token	*extract_token(char *line, int *pos, t_token *prev_token, t_error *error)
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
		free (token);
		return (NULL);
	}
	return (token);
}

t_error	append_token_to_list(t_dlst **tokens_list, t_token *token, t_error *error)
{
	t_dlst	*new_node;

	if (token == NULL)
		return (*error);
	new_node = ft_dlstnew(token);
	if (new_node == NULL)
	{
		free_token_content(token);
		free(token);
		free_toklist(tokens_list);
		*error = critical;
		return (critical); // ERROR MESSAGE + FREE;
	}
	ft_dlstadd_back(tokens_list, new_node);
	return (success);
}

// In case of error scan_line free it's memory but it's the responsibiliy of the caller to free line.
t_dlst	*scan_line(char *line, t_error *error)
{
	int	pos;
	t_token	*token;
	t_dlst	*tokens_list;
	t_bool	cat_prev;

	pos = 0;
	token = NULL;
	tokens_list = NULL;
	while (1)
	{
		if (skip_spaces(line, &pos))
			cat_prev = false;
		else
			cat_prev = true;
		if (line[pos] == '\0')
			break ;
		token = extract_token(line, &pos, token, error);
		if (!*error && token)
			token->cat_prev = cat_prev;
		if (append_token_to_list(&tokens_list, token, error) != success)
			return (NULL);
	}
	return (tokens_list);
}
//
//int	main(int argc, char *argv[])
//{
//	t_dlst	*tokens_list;
//	t_error	error;
//
//	if (argc != 2)
//		return (EXIT_FAILURE);
//	error = success;
//	tokens_list = scan_line(argv[1], &error);
//	if (error == critical)
//		return (EXIT_FAILURE); // HANDLE ERROR CORRECTLY
//	else if (error == recoverable)
//		return (EXIT_FAILURE); // HANDLE ERROR CORRECTLY
//	printf("tokens_list size == %d\n", ft_dlstsize(tokens_list));
//	print_toklist(tokens_list, 0);
//	return (EXIT_SUCCESS);
//}
