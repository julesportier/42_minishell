/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token_extract.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: juportie <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/28 16:21:32 by juportie          #+#    #+#             */
/*   Updated: 2025/05/14 11:31:45 by juportie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include <stdio.h>
#include "../../libft/src/libft.h"
#include "../minishell.h"
#include "parsing.h"
#include "lexer.h"

t_error	extract_quotes(t_token *token, char *line, int *pos)
{
	int		start;
	char	quote;

	quote = line[*pos];
	advance(1, pos);
	start = *pos;
	while (line[*pos] != quote && line[*pos] != '\0')
		advance(1, pos);
	if (line[*pos] == '\0')
	{
		ft_putstr_fd("minishell: syntax error: unclosed quote\n", 2);
		return (recoverable);
	}
	if (quote == '"')
		token->type = double_quotes;
	else
		token->type = literal;
	token->str = ft_substr(line, start, *pos - start);
	if (token->str == NULL)
		return (err_perror_alloc(critical, NULL));
	advance(1, pos);
	return (success);
}

t_error	extract_literal(t_token *token, char *line, int *pos)
{
	int	start;

	start = *pos;
	while (!is_blank(line[*pos])
		&& !is_quote(line[*pos])
		&& !is_operator(&line[*pos])
		&& !is_expanding(line[*pos])
		&& line[*pos] != '\0')
		advance(1, pos);
	token->type = literal;
	token->str = ft_substr(line, start, *pos - start);
	if (token->str == NULL)
		return (err_perror_alloc(critical, NULL));
	return (success);
}

t_error	extract_variable_identifier(t_token *token, char *line, int *pos)
{
	int	start;

	advance(1, pos);
	start = *pos;
	while (!is_blank(line[*pos])
		&& !is_quote(line[*pos])
		&& !is_operator(&line[*pos])
		&& !is_expanding(line[*pos])
		&& line[*pos - 1] != '?'
		&& line[*pos] != '\0')
		advance(1, pos);
	if (*pos - start > 0)
	{
		token->type = variable;
		token->str = ft_substr(line, start, *pos - start);
	}
	else
	{
		token->type = literal;
		token->str = ft_strdup("$");
	}
	if (token->str == NULL)
		return (err_perror_alloc(critical, NULL));
	return (success);
}

t_error	extract_operator(t_token *token, char *line, int *pos)
{
	if (match(&line[*pos], '|'))
		consume_two_char(token, or, pos);
	else if (line[*pos] == '|')
		consume_one_char(token, pipeline, pos);
	else if (match(&line[*pos], '&'))
		consume_two_char(token, and, pos);
	else if (line[*pos] == '(' || line[*pos] == ')')
		return (extract_grouping(token, line, pos));
	else if (match(&line[*pos], '<'))
		consume_two_char(token, heredoc, pos);
	else if (line[*pos] == '<')
		consume_one_char(token, redir_input, pos);
	else if (match(&line[*pos], '>'))
		consume_two_char(token, append_output, pos);
	else if (line[*pos] == '>')
		consume_one_char(token, redir_output, pos);
	return (success);
}

t_error	extract_expanding(t_token *token, char *line, int *pos)
{
	if (line[*pos] == '$')
		return (extract_variable_identifier(token, line, pos));
	else if (line[*pos] == '*')
	{
		consume_one_char(token, wildcard, pos);
		token->str = ft_strdup("*");
		if (token->str == NULL)
			return (err_perror_alloc(critical, NULL));
	}
	return (success);
}
