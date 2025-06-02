/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token_extract_heredoc.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: juportie <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/22 11:14:52 by juportie          #+#    #+#             */
/*   Updated: 2025/05/22 11:19:52 by juportie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include "../../libft/src/libft.h"
#include "../minishell.h"
#include "lexer.h"

static t_error	extract_quoted_delimiter(t_token *token, char *line, int *pos)
{
	int	start;
	char	quote;

	quote = line[*pos];
	advance(1, pos);
	start = *pos;
	while (line[*pos] != quote && line[*pos] != '\0')
		advance(1, pos);
	if (line[*pos] == '\0')
		return (print_syntax_error("unclosed ", quote, recoverable));
	token->type = heredoc_lit;
	token->str = ft_substr(line, start, *pos - start); // Returns "\0" for "" as input.
	if (token->str == NULL)
		return (critical);
	advance(1, pos);
	return (success);
}

static t_error	extract_unquoted_delimiter(t_token *token, char *line, int *pos)
{
	int	start;

	start = *pos;
	while (!is_blank(line[*pos])
		&& !is_quote(line[*pos])
		&& !is_operator(&line[*pos])
		&& line[*pos] != '\0')
		advance(1, pos);
	token->type = heredoc_exp;
	token->str = ft_substr(line, start, *pos - start); // Returns "\0" for "" as input.
	if (token->str == NULL)
		return (critical);
	return (success);
}

t_error	extract_delimiter(t_token *token, char *line, int *pos, t_error *error)
{
	if (is_quote(line[*pos]))
		*error = extract_quoted_delimiter(token, line, pos);
	else
		*error = extract_unquoted_delimiter(token, line, pos);
	return (*error);
}
