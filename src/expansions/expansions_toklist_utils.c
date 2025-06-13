/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expansions_toklist_utils.c                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: juportie <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/26 10:25:15 by juportie          #+#    #+#             */
/*   Updated: 2025/06/09 17:51:16 by juportie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../libft/src/libft.h"
#include "../minishell.h"
#include "../general_utils/utils.h"
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
		return (err_perror_alloc(critical, error));
	return (*error);
}

t_error	merge_next_token(t_dlst *token, t_error *error)
{
	set_toklist_str(
		token,
		free_strjoin(
			get_toklist_str(token), get_toklist_str(token->next), true, false));
	ft_dlstremove(token->next, free_token_content, free);
	if (get_toklist_str(token) == NULL)
		return (err_print_alloc(critical, error));
	return (success);
}

t_error	concatenate_toklist(t_dlst **toklist, t_error *error)
{
	t_dlst	*token;

	if (!toklist)
		return (success);
	token = *toklist;
	while (token && token->next)
	{
		while (token->next
			&& get_toklist_type(token) == literal
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
