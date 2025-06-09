/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expansions_wildcard_concatenate.c                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: juportie <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/26 10:25:15 by juportie          #+#    #+#             */
/*   Updated: 2025/06/09 17:34:45 by juportie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../parsing/parsing.h"
#include "expansions.h"

static t_dlst	*merge_wildcard(t_dlst *token, t_error *error)
{
	while (token->prev && get_toklist_cat_prev(token) == true)
		token = token->prev;
	set_toklist_type(token, wildcard);
	while (token
		&& token->next
		&& get_toklist_cat_prev(token->next) == true)
	{
		if (merge_next_token(token, error) != success)
			return (NULL);
	}
	return (token);
}

t_error	concatenate_wildcards(t_dlst **toklist, t_error *error)
{
	t_dlst	*token;

	token = *toklist;
	while (token)
	{
		if (get_toklist_type(token) == wildcard)
		{
			token = merge_wildcard(token, error);
			if (*error)
				return (*error);
		}
		token = token->next;
		if (token == NULL)
			break ;
	}
	return (success);
}
