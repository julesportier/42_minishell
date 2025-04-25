/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: juportie <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/22 10:22:36 by juportie          #+#    #+#             */
/*   Updated: 2025/04/25 13:58:49 by juportie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include <stdio.h>
#include "../../libft/src/libft.h"
#include "../minishell.h"
#include "parsing.h"
#include "lexer.h"

static int	extract_quotes(t_word *token, char *line, int *pos)
{
	int	start;
	char	quote;

	quote = line[*pos];
	advance(1, pos);
	start = *pos;
	while (line[*pos] != quote && line[*pos] != '\0')
		advance(1, pos);
	if (line[*pos] == '\0')
	{
		printf("minishell: syntax error: unclosed '%c'\n", quote); // PUT THIS IN A SYNTAX ERROR FILE
		return (ERROR);
	}
	if (quote == '"')
		token->type = double_quotes;
	else
		token->type = literal;
	token->str = ft_strdup(ft_substr(line, start, *pos - start)); // Returns "\0" for "" as input.
	if (token->str == NULL)
		return (CRIT_ERROR);
	advance(1, pos);
	return (SUCCESS);
}

static int	extract_plain(t_word *token, char *line, int *pos)
{
	int	start;

	start = *pos;
	while (!is_meta(&line[*pos]) && !is_separator(line[*pos]) && line[*pos] != '\0')
		advance(1, pos);
	token->type = plain;
	token->str = ft_strdup(ft_substr(line, start, *pos - start)); // Returns "\0" for "" as input.
	if (token->str == NULL)
		return (CRIT_ERROR);
	return (SUCCESS);
}

static int	extract_identifier(t_word *token, char *line, int *pos)
{
	int	start;

	advance(1, pos);
	start = *pos;
	while (!is_separator(line[*pos]) && !is_meta(&line[*pos]) && line[*pos] != '\0')
		advance(1, pos);
	if (*pos - start > 0)
	{
		token->type = variable;
		token->str = ft_strdup(ft_substr(line, start, *pos - start)); // Returns "\0" for "" as input.
	}
	else
	{
		token->type = literal;
		token->str = ft_strdup("$");
	}
	if (token->str == NULL)
		return (CRIT_ERROR);
	return (SUCCESS);
}

static void	extract_semantic_double(t_word *token, enum e_token_type type, int *pos)
{
	advance(2, pos);
	token->str = NULL;
	token->type = type;
}

static void	extract_semantic_single(t_word *token, enum e_token_type type, int *pos)
{
	advance(1, pos);
	token->str = NULL;
	token->type = type;
}

// Needs error pointer to discrimine between critical and recoverable errors.
static t_word	*extract_token(char *line, int *pos, t_error *error)
{
	t_word	*token;

	token = malloc(sizeof(t_word));
	if (token == NULL)
	{
		*error = critical;
		return (NULL);
	}
	if (DEBUG)
		printf("extract token: token addr == %p\n", token);

	if (skip_spaces(line, pos))
		token->cat_prev = false;
	else
		token->cat_prev = true;


	if (line[*pos] == '"'  || line[*pos] == '\'')
		*error = extract_quotes(token, line, pos);
	else if (line[*pos] == '$')
	{
		*error = extract_identifier(token, line, pos);
		if (DEBUG)
			printf("extract token: extract_identidier\n");
	}
	else if (line[*pos] == '*')
		extract_semantic_single(token, wildcard, pos);
	else if (line[*pos] == '|')
	{
		if (line[*pos + 1] == '|')
			extract_semantic_double(token, or, pos);
		else
			extract_semantic_single(token, pipeline, pos);
	}
	else if (line[*pos] == '&' && line[*pos + 1] == '&')
		extract_semantic_double(token, and, pos);
	else if (line[*pos] == '(')
		extract_semantic_single(token, left_parenthesis, pos);
	else if (line[*pos] == ')')
		extract_semantic_single(token, right_parenthesis, pos);
	else if (line[*pos] == '<')
	{
		if (line[*pos + 1] == '<')
			extract_semantic_double(token, heredoc, pos);
		else
			extract_semantic_single(token, redir_input, pos);
	}
	else if (line[*pos] == '>')
	{
		if (line[*pos + 1] == '>')
			extract_semantic_double(token, append_output, pos);
		else
			extract_semantic_single(token, redir_output, pos);
	}
	else
		*error = extract_plain(token, line, pos);
	if (*error)
	{
		free (token);
		return (NULL);
	}
	return (token);
}



int	main(int argc, char *argv[])
{
	t_word	*token;
	t_error	error;

	if (argc != 2)
		return (EXIT_FAILURE);
	printf("argv[1] == %s\n", argv[1]);
	error = success;
	int	pos = 0;
	token = extract_token(argv[1], &pos, &error);
	printf("error == %d\n", error);
	if (error == critical)
		return (EXIT_FAILURE); // HANDLE ERROR CORRECTLY
	else if (error == recoverable)
		return (EXIT_FAILURE); // HANDLE ERROR CORRECTLY
	printf("token addr == %p\n", token);
	printf("token: %d\n", token->type);
	return (EXIT_SUCCESS);
}



//t_word	*scan_line(char *line, t_error *error)
//{
//	int	pos;
//	t_word	*token;
//	t_word	*tokens_list;
//
//	if (line == NULL)
//		return (NULL);
//	pos = 0;
//	tokens_list = NULL;
//	while (line[pos] != '\0')
//	{
//		token = extract_token(line, &pos, error);
//		if (*error == critical)
//			return (NULL); // CRITICAL FAILURE. FREE EVERYTHING AND EXIT.
//		if (token == NULL) // not an error, just a string without tokens (eg only spaces)
//			break ;
//		if (append_token_to_list(&tokens_list, token) == -1)
//			return (NULL); // CRITICAL FAILURE. FREE EVERYTHING AND EXIT.
//	}
//	return (tokens_list);
//}
//
//int	main(int argc, char *argv[])
//{
//	t_word	*tokens_list;
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
//	else if (tokens_list == NULL)
//		return (EXIT_SUCCESS);
//
//
//	return (EXIT_SUCCESS);
//}
