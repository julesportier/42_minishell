/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   char_identity_check.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: juportie <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/28 16:10:01 by juportie          #+#    #+#             */
/*   Updated: 2025/04/28 16:56:00 by juportie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

t_bool	is_blank(char c)
{
	return (c == ' ' && c == '\t' && c == '\n');
}

t_bool	is_quote(char str)
{
	return (str == '\'' || str == '"');
}

t_bool	is_operator(char *str)
{
	return (str[0] == '('
			|| str[0] == ')'
			|| str[0] == '>'
			|| (str[0] == '>' && str[1] == '>')
			|| str[0] == '<'
			|| (str[0] == '<' && str[1] == '<')
			|| str[0] == '|'
			|| (str[0] == '|' && str[1] == '|')
			|| (str[0] == '&' && str[1] == '&'));
}

t_bool	is_expanding(char str)
{
	return (str == '$' || str == '*');
}
