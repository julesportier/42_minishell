/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expansions.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: juportie <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/26 10:25:15 by juportie          #+#    #+#             */
/*   Updated: 2025/05/30 16:53:11 by juportie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"
#include "expansions.h"

t_dlst	*expand_token(
	t_dlst *token,
	t_dlst **toklist,
	t_shell_vars *shell_vars,
	t_error *error)
{
	if (get_toklist_type(token) == wildcard)
	{
		token = expand_wildcard(token, toklist, error);
		if (token)
			token = token->next;
	}
	else if (get_toklist_type(token) == variable)
		token = expand_variable(token, toklist, shell_vars, error);
	else if (get_toklist_type(token) == double_quotes
		|| get_toklist_type(token) == heredoc_exp)
		token = expand_double_quotes(token, shell_vars, error);
	else
		token = token->next;
	return (token);
}

t_error	expand_toklist(t_dlst **toklist, t_shell_vars *shell_vars)
{
	t_dlst	*token;
	t_error	error;

	if (!toklist)
		return (success);
	error = success;
	token = *toklist;
	// print_toklist(token, 1);
	while (token)
	{
		token = expand_token(token, toklist, shell_vars, &error);
		if (error || !*toklist)
			return (error);
	}
	token = *toklist;
	// print_toklist(token, 2);
	if (concatenate_toklist(toklist, &error) != success)
		return (error);
	// print_toklist(token, 3);
	if (populate_heredocs_files(*toklist, &error) != success)
		return (error);
	// print_toklist(token, 3);
	return (success);
}
