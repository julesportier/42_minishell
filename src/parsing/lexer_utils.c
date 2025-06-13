/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: juportie <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/25 09:45:55 by juportie          #+#    #+#             */
/*   Updated: 2025/04/28 16:56:10 by juportie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../libft/src/libft.h"
#include "../minishell.h"
#include "lexer.h"

void	advance(int i, int *pos)
{
	(*pos) += i;
}

t_bool	match(char *str, char match)
{
	return (str[0] == match && str[1] == match);
}

t_bool	skip_spaces(char *line, int *pos)
{
	t_bool	skipped;

	skipped = false;
	while (is_blank(line[*pos]) && line[*pos] != '\0')
	{
		skipped = true;
		advance(1, pos);
	}
	return (skipped);
}

t_token	*alloc_token(t_error *error)
{
	t_token	*token;

	token = ft_calloc(1, sizeof(t_token));
	if (token == NULL)
		return (null_perror_alloc(critical, error));
	return (token);
}
