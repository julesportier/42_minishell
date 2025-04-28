/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: juportie <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/22 10:22:36 by juportie          #+#    #+#             */
/*   Updated: 2025/04/28 16:57:31 by juportie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include <stdio.h>
#include "../../libft/src/libft.h"
#include "../minishell.h"
#include "parsing.h"
#include "lexer.h"

// Needs error pointer to discrimine between critical and recoverable errors.
static t_word	*extract_token(char *line, int *pos, t_bool cat_prev, t_error *error)
{
	t_word	*token;

	token = malloc(sizeof(t_word));
	if (token == NULL)
	{
		*error = critical;
		return (NULL);
	}
	token->cat_prev = cat_prev;
	if (is_quote(line[*pos]))
		*error = extract_quotes(token, line, pos);
	else if (is_operator(&line[*pos]))
		extract_operator(token, line, pos);
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

static int	append_token_to_list(t_dlst **tokens_list, t_word *token)
{
	t_dlst	*new_node;

	new_node = ft_dlstnew(token);
	if (new_node == NULL)
		return (CRIT_ERROR); // ERROR MESSAGE + FREE;
	ft_dlstadd_back(tokens_list, new_node);
	return (SUCCESS);
}

t_dlst	*scan_line(char *line, t_error *error)
{
	int	pos;
	t_word	*token;
	t_dlst	*tokens_list;
	t_bool	cat_prev;

	if (line == NULL)
		return (NULL);
	pos = 0;
	tokens_list = NULL;
	while (1)
	{
		if (skip_spaces(line, &pos))
			cat_prev = false;
		else
			cat_prev = true;
		if (line[pos] == '\0')
			break ;
		token = extract_token(line, &pos, cat_prev, error);
		if (*error == critical)
			return (NULL); // CRITICAL FAILURE. FREE EVERYTHING AND EXIT.
		if (append_token_to_list(&tokens_list, token) == -1)
			return (NULL); // CRITICAL FAILURE. FREE EVERYTHING AND EXIT.
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
//	int	i = 0;
//	while (tokens_list)
//	{
//		printf("token%d == '%s' ; ", i, ((t_word *)tokens_list->content)->str);
//		printf("type == %d\n", ((t_word *)tokens_list->content)->type);
//		tokens_list = tokens_list->next;
//		i++;
//	}
//	return (EXIT_SUCCESS);
//}
