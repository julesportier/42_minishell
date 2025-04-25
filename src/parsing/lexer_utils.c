/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: juportie <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/25 09:45:55 by juportie          #+#    #+#             */
/*   Updated: 2025/04/25 13:23:47 by juportie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

t_bool	is_separator(char c)
{
	return (c == ' ');
}

t_bool	is_meta(char *str)
{
	return (str[0] == '\''
			|| str[0] == '"'
			|| str[0] == '$'
			|| str[0] == '*'
			|| (str[0] == '|' && str[1] == '|')
			|| (str[0] == '&' && str[1] == '&')
			|| str[0] == '|'
			|| str[0] == '('
			|| str[0] == ')'
			|| str[0] == '>'
			|| (str[0] == '>' && str[1] == '>')
			|| str[0] == '<'
			|| (str[0] == '<' && str[1] == '<'));
}

void	advance(int i, int *pos)
{
	(*pos) += i;
}

t_bool	skip_spaces(char *line, int *pos)
{
	t_bool	skipped;

	skipped = false;
	while (is_separator(line[*pos]) && line[*pos] != '\0')
	{
		skipped = true;
		advance(1, pos);
	}
	return (skipped);
}
