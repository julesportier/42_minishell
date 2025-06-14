/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expansions_utils.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: juportie <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/26 10:25:15 by juportie          #+#    #+#             */
/*   Updated: 2025/05/30 17:06:39 by juportie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../libft/src/libft.h"
#include "../minishell.h"
#include "expansions.h"
#include "../parsing/lexer.h"

static t_dlst	*insert_expanded_token(t_dlst *token, char *str, t_error *error)
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

t_error	place_expansion_result(
	t_dlst **token,
	enum e_token_type type,
	char *new_str,
	t_error *error)
{
	if (get_toklist_type(*token) == type)
		replace_token_content(
			(*token)->content,
			literal, new_str, error);
	else
		*token = insert_expanded_token(*token, new_str, error);
	return (*error);
}

char	*join_char_free(char *str, char c, t_error *error)
{
	char	*new_str;
	int		i;

	new_str = ft_calloc(ft_strlen(str) + 2, sizeof(char));
	if (!new_str)
		return (null_perror_alloc(critical, error));
	if (new_str)
	{
		i = 0;
		while (str && str[i])
		{
			new_str[i] = str[i];
			i++;
		}
		new_str[i] = c;
		new_str[i + 1] = '\0';
	}
	free(str);
	return (new_str);
}
