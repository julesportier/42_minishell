/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expansions_toklist_utils.c                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: juportie <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/26 10:25:15 by juportie          #+#    #+#             */
/*   Updated: 2025/05/30 17:05:32 by juportie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../libft/src/libft.h"
#include "../minishell.h"
#include "../parsing/parsing.h"

t_error	replace_token_content(
	t_token *token,
	enum e_token_type type,
	char *str,
	t_error *error)
{
	token->type = type;
	free(token->str);
	token->str = ft_strdup(str);
	if (token->str == NULL)
		*error = critical;
	return (*error);
}

static t_error	merge_next_token(t_dlst *token, t_error *error)
{
	set_toklist_str(
		token,
		ft_strjoin(get_toklist_str(token), get_toklist_str(token->next)));
	if (get_toklist_str(token) == NULL)
	{
		*error = critical;
		return (critical);
	}
	ft_dlstremove(token->next, free_token_content, free);
	return (success);
}

t_error	concatenate_toklist(t_dlst **toklist, t_error *error)
{
	t_dlst *token;

	if (!toklist)
		return (success);
	token = *toklist;
	while (token && token->next)
	{
		if (get_toklist_type(token) == literal
			&& get_toklist_type(token->next) == literal
			&& get_toklist_cat_prev(token->next) == true)
		{
			if (merge_next_token(token, error) != success)
				return (*error);
		}
		token = token->next;
	}
	return (success);
}
