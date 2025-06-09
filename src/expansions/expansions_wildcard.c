/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expansions_wildcard.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: juportie <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/26 10:25:15 by juportie          #+#    #+#             */
/*   Updated: 2025/06/09 17:50:24 by juportie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <dirent.h>
#include "../../libft/src/libft.h"
#include "../parsing/parsing.h"
#include "expansions.h"

static t_dlst	*match_cwd_content_list(
	char *wildcard_str,
	t_dlst *cwd_content_list,
	t_error *error)
{
	t_dlst	*head;
	t_dlst	*next;
	char	*trimmed_wildcard_str;

	head = cwd_content_list;
	trimmed_wildcard_str = trim_wildcard_str(wildcard_str, error);
	if (*error)
		return (NULL);
	while (cwd_content_list)
	{
		next = cwd_content_list->next;
		if (!match_glob_entry(
				trimmed_wildcard_str, get_toklist_str(cwd_content_list)))
		{
			if (cwd_content_list->prev == NULL)
				head = head->next;
			ft_dlstremove(cwd_content_list, free_token_content, free);
		}
		cwd_content_list = next;
	}
	free(trimmed_wildcard_str);
	return (head);
}

static t_dlst	*set_wildcard_to_literal(t_dlst *token)
{
	((t_token *)(token->content))->type = literal;
	return (token);
}

static void	remove_token(t_dlst **toklist, t_dlst *token)
{
	if (token->prev == NULL)
		*toklist = (*toklist)->next;
	ft_dlstremove(token, free_token_content, free);
}

static t_dlst	*expand_wildcard_token(
	t_dlst **toklist,
	t_dlst *token,
	t_error *error)
{
	t_dlst	*cwd_content_list;
	t_dlst	*sublist_last;

	cwd_content_list = create_filtered_cwd_content_list(token, error);
	if (*error)
		return (NULL);
	else if (cwd_content_list == NULL)
		return (set_wildcard_to_literal(token));
	cwd_content_list = match_cwd_content_list(
			get_toklist_str(token), cwd_content_list, error);
	if (cwd_content_list)
	{
		if (append_junk_to_cwd_list(
				cwd_content_list, get_toklist_str(token), error) != success)
		{
			free_toklist(&cwd_content_list);
			return (NULL);
		}
		sort_list(&cwd_content_list);
		sublist_last = ft_dlstlast(cwd_content_list);
		ft_dlstinsert_sublist_next(token, cwd_content_list);
		remove_token(toklist, token);
		return (sublist_last);
	}
	return (token);
}

t_dlst	*expand_wildcards(t_dlst **toklist, t_error *error)
{
	t_dlst	*token;

	if (concatenate_wildcards(toklist, error) != success)
		return (NULL);
	token = *toklist;
	while (token)
	{
		// if (DEBUG)
		// 	printf("check if expanding token: '%s'...\n", get_toklist_str(token));
		if (get_toklist_type(token) == wildcard)
		{
			// if (DEBUG)
			// 	printf("expanding token: '%s'...\n", get_toklist_str(token));
			token = expand_wildcard_token(toklist, token, error);
			if (*error)
				return (NULL);
		}
		token = token->next;
	}
	return (token);
}
