/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token_extract_grouping.c                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: juportie <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/14 11:16:56 by juportie          #+#    #+#             */
/*   Updated: 2025/05/14 12:49:45 by juportie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parsing.h"
#include "lexer.h"

static t_error	check_nesting(int nesting_level, char next_char, enum e_token_type token_type)
{
	if (next_char == '\0' && nesting_level != 0)
		return (print_syntax_error("missing ", right_parenthesis, recoverable));
	else if (nesting_level < 0)
		return (print_syntax_error("unexpected ", token_type, recoverable));
	else
		return (success);
}

t_error extract_grouping(t_token *token, char *line, int *pos)
{
	static int	nesting_level;

	if (*pos == 0)
		nesting_level = 0;
	if (line[*pos] == '(')
	{
		extract_one_char(token, left_parenthesis, pos);
		++nesting_level;
	}
	else if (line[*pos] == ')')
	{
		extract_one_char(token, right_parenthesis, pos);
		--nesting_level;
	}
	return (check_nesting(nesting_level, line[*pos], token->type));
}
