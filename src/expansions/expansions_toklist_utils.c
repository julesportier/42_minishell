/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expansions_toklist_utils.c                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: juportie <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/26 10:25:15 by juportie          #+#    #+#             */
/*   Updated: 2025/05/30 15:40:47 by juportie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../libft/src/libft.h"
#include "../minishell.h"
#include "../parsing/parsing.h"
#include "../parsing/lexer.h"

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

t_dlst	*insert_expanded_token(t_dlst *token, char *str, t_error *error)
{
	t_token	*new_token;
	t_dlst	*new_node;

	new_token = alloc_token(error);
	if (*error)
		return (NULL);
	if (replace_token_content(new_token, literal, str, error))
		return (NULL);
	new_node = ft_dlstnew(new_token);
	if (!new_node)
	{
		*error = critical;
		return (NULL);
	}
	ft_dlstinsert_next(&token, new_node);
	return (token->next);
}

void	remove_token(t_dlst **token, t_dlst **toklist)
{
	t_dlst	*temp_node;

	if (toklist && *toklist == *token)
		*toklist = (*toklist)->next;
	temp_node = *token;
	*token = (*token)->next;
	ft_dlstremove(temp_node, free_token_content, free);
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
