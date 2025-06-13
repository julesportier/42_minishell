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
#include "../error_handling/errors.h"
#include "lexer.h"
#include "../expansions/expansions.h"

static t_bool	increment_quotes(
	char c,
	int *d_quotes,
	int *s_quotes,
	t_token *token)
{
	if (is_quote(c))
	{
		if (c == '"')
			++*d_quotes;
		else
			++*s_quotes;
		token->type = heredoc_lit;
		return (true);
	}
	else
		return (false);
}

static t_error	set_empty(
	t_token *token,
	int d_quotes,
	int s_quotes,
	t_error *error)
{
	if (!token->str)
	{
		if (d_quotes || s_quotes)
		{
			token->str = ft_strdup("");
			if (!token->str)
				return (err_perror_alloc(critical, error));
		}
		else
			return (print_syntax_error(
					"unexpected token ", heredoc, recoverable, error));
	}
	return (success);
}

static t_error	check_quotes(t_token *token, int d_quotes, int s_quotes)
{
	if (d_quotes % 2 != 0 || s_quotes % 2 != 0)
	{
		free(token->str);
		token->str = NULL;
		return (recoverable);
	}
	return (success);
}

static t_error	check_delimiter(
	t_token *token,
	int d_quotes,
	int s_quotes,
	t_error *error)
{
	if (check_quotes(token, d_quotes, s_quotes) != success)
		return (print_syntax_error(
				"unexpected token ", heredoc, recoverable, error));
	if (set_empty(token, d_quotes, s_quotes, error) != success)
		return (*error);
	return (success);
}

t_error	extract_delimiter(
	t_token *token,
	char *line,
	int *pos,
	t_error *error)
{
	int	d_quotes;
	int	s_quotes;

	d_quotes = 0;
	s_quotes = 0;
	token->type = heredoc_exp;
	while (!is_blank(line[*pos])
		&& !is_operator(&line[*pos])
		&& line[*pos] != '\0')
	{
		if (!increment_quotes(line[*pos], &d_quotes, &s_quotes, token))
		{
			token->str = join_char_free(token->str, line[*pos], error);
			if (!token->str)
				return (*error);
		}
		advance(1, pos);
	}
	if (check_delimiter(token, d_quotes, s_quotes, error) != success)
		return (*error);
	return (success);
}
