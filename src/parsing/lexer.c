/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: juportie <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/22 10:22:36 by juportie          #+#    #+#             */
/*   Updated: 2025/04/22 17:53:46 by juportie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include <stdio.h>
#include "../../libft/src/libft.h"
#include "../minishell.h"
#include "parsing.h"

// MOVE THIS FUNC AWAY ?
static int	is_separator(char c)
{
	return (c == ' ');
}

static void	advance_scan(int *pos)
{
	(*pos)++;
}

void	extract_wildcard(t_word *token, int *pos)
{
	(*pos)++;
	token->type = wildcard;
	token->str = NULL;
}

void	extract_quotes(t_word *token, char *line, int *pos, t_error *error)
{
	int	start;
	char	quote;

	start = *pos;
	quote = line[*pos];
	while (line[*pos] != quote && line[*pos] != '\0')
		(*pos)++;
	if (line[*pos] == '\0')
	{
		*error = recoverable;
		printf("minishell: syntax error: unclosed '%c'\n", quote); // PUT THIS IN A SYNTAX ERROR FILE
		return ;
	}
	if (quote == '"')
		token->type = double_quotes;
	else
		token->type = literal;
	token->str = ft_strdup(ft_substr(line, start, *pos - start)); // Returns "\0" for "" as input.
	if (token->str == NULL)
	{
		*error = critical;
		return ;
	}
}

t_word	*extract_token(char *line, int *pos, t_error *error)
{
	t_word	*token;

	token = malloc(sizeof(t_word));
	if (token == NULL)
	{
		*error = critical;
		return (NULL);
	}
	token->cat_prev = true;
	while (is_separator(line[*pos]) && line[*pos] != '\0')
	{
		token->cat_prev = false;
		pos++;
	}
	if (line[*pos] == '"' || line[*pos] == '\'')
		extract_quotes(token, line, pos, error);
	else if (line[*pos] == '*')
		extract_wildcard(token, pos);
	else if (line[*pos] == '(')
		extract_left_parenthesis(token, pos);

	if (error)
	{
		free (token);
		token = NULL;
	}

	return (token);
}






t_word	*scan_line(char *line, t_error *error)
{
	int	pos;
	t_word	*token;
	t_word	*tokens_list;

	if (line == NULL)
		return (NULL);
	pos = 0;
	tokens_list = NULL;
	while (line[pos] != '\0')
	{
		token = extract_token(line, &pos, error);
		if (*error == critical)
			return (NULL); // CRITICAL FAILURE. FREE EVERYTHING AND EXIT.
		if (token == NULL) // not an error, just a string without tokens (eg only spaces)
			break ;
		if (append_token_to_list(&tokens_list, token) == -1)
			return (NULL); // CRITICAL FAILURE. FREE EVERYTHING AND EXIT.
	}
	return (tokens_list);
}

int	main(int argc, char *argv[])
{
	t_word	*tokens_list;
	t_error	error;

	if (argc != 2)
		return (EXIT_FAILURE);
	error = success;
	tokens_list = (argv[1], &error);
	if (error == critical)
		return (EXIT_FAILURE); // HANDLE ERROR CORRECTLY
	else if (error == recoverable)
		return (EXIT_FAILURE); // HANDLE ERROR CORRECTLY
	else if (tokens_list == NULL)
		return (EXIT_SUCCESS);


	return (EXIT_SUCCESS);
}
