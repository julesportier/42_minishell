/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expansions.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: juportie <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/26 10:25:15 by juportie          #+#    #+#             */
/*   Updated: 2025/06/09 17:20:46 by juportie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"
#include "expansions.h"

static t_dlst	*expand_token(
	t_dlst *token,
	t_dlst **toklist,
	t_shell_vars *shell_vars,
	t_error *error)
{
	if (get_toklist_type(token) == variable)
		token = expand_variable(token, toklist, shell_vars, error);
	else if (get_toklist_type(token) == double_quotes
		|| get_toklist_type(token) == heredoc_exp)
		token = expand_double_quotes(token, shell_vars, error);
	else
		token = token->next;
	return (token);
}

t_error	expand_toklist(
	t_dlst **toklist,
	t_shell_vars *shell_vars,
	t_error *error)
{
	t_dlst	*token;

	if (!toklist)
		return (*error);
	token = *toklist;
	while (token)
	{
		token = expand_token(token, toklist, shell_vars, error);
		if (*error || !*toklist)
			return (*error);
	}
	token = *toklist;
	token = expand_wildcards(toklist, error);
	if (*error)
		return (*error);
	if (concatenate_toklist(toklist, error) != success)
		return (*error);
	if (populate_heredocs_files(*toklist, error) != success)
		return (*error);
	return (success);
}
