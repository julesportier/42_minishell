/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token_extract.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: juportie <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/28 16:21:32 by juportie          #+#    #+#             */
/*   Updated: 2025/05/06 15:24:54 by juportie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include <stdio.h>
#include "../../libft/src/libft.h"
#include "../minishell.h"
#include "parsing.h"
#include "lexer.h"

int	extract_quotes(t_token *token, char *line, int *pos)
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
	token->str = ft_substr(line, start, *pos - start); // Returns "\0" for "" as input.
	if (token->str == NULL)
		return (CRIT_ERROR);
	advance(1, pos);
	return (SUCCESS);
}

int	extract_literal(t_token *token, char *line, int *pos)
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
	token->str = ft_substr(line, start, *pos - start); // Returns "\0" for "" as input.
	if (token->str == NULL)
		return (CRIT_ERROR);
	return (SUCCESS);
}

int	extract_variable_identifier(t_token *token, char *line, int *pos)
{
	int	start;

	advance(1, pos);
	start = *pos;
	while (!is_blank(line[*pos])
		&& !is_quote(line[*pos])
		&& !is_operator(&line[*pos])
		&& !is_expanding(line[*pos])
		&& line[*pos] != '\0')
		advance(1, pos);
	if (*pos - start > 0)
	{
		token->type = variable;
		token->str = ft_substr(line, start, *pos - start); // Returns "\0" for "" as input.
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

void	extract_operator(t_token *token, char *line, int *pos)
{
	if (match(&line[*pos], '|'))
		extract_two_char(token, or, pos);
	else if (line[*pos] == '|')
		extract_one_char(token, pipeline, pos);
	else if (match(&line[*pos], '&'))
		extract_two_char(token, and, pos);
	else if (line[*pos] == '(')
		extract_one_char(token, left_parenthesis, pos);
	else if (line[*pos] == ')')
		extract_one_char(token, right_parenthesis, pos);
	else if (match(&line[*pos], '<'))
		extract_two_char(token, heredoc, pos);
	else if (line[*pos] == '<')
		extract_one_char(token, redir_input, pos);
	else if (match(&line[*pos], '>'))
		extract_two_char(token, append_output, pos);
	else if (line[*pos] == '>')
		extract_one_char(token, redir_output, pos);
}

int	extract_expanding(t_token *token, char *line, int *pos)
{
	if (line[*pos] == '$')
		return (extract_variable_identifier(token, line, pos));
	else if (line[*pos] == '*')
		extract_one_char(token, wildcard, pos);
	return (SUCCESS);
}
