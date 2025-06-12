/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tree_build_grouping.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: juportie <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/06 09:44:07 by juportie          #+#    #+#             */
/*   Updated: 2025/05/22 11:47:23 by juportie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>
#include "../../libft/src/libft.h"
#include "../minishell.h"
#include "parsing.h"
#include "tree_build.h"

static t_bool	is_valid_grouping(t_dlst *toklist, t_error *error)
{
	t_dlst	*last;

	last = ft_dlstlast(toklist);
	if (get_toklist_type(toklist) != left_parenthesis)
	{
		print_syntax_error("ill-formed grouping, unexpected token ",
			get_toklist_type(toklist), recoverable, error);
		return (false);
	}
	else if (get_toklist_type(last) != right_parenthesis)
	{
		print_syntax_error("ill-formed grouping, unexpected token ",
			get_toklist_type(last), recoverable, error);
		return (false);
	}
	else if (toklist->next == last)
	{
		print_syntax_error("empty grouping, unexpected token ",
			get_toklist_type(toklist), recoverable, error);
		return (false);
	}
	return (true);
}

static t_bool	is_grouping(t_dlst *toklist, t_error *error)
{
	int	nesting_level;
	t_bool	parenthesis;
	t_dlst	*token;

	nesting_level = 0;
	parenthesis = false;
	token = toklist;
	while (token)
	{
		nesting_level = update_parenthesis_nesting_level(token, nesting_level);
		if (nesting_level > 0)
			parenthesis = true;
		if (check_nesting_level(nesting_level, token) != success)
		{
			print_syntax_error(
				"unexpected token ", get_toklist_type(token), recoverable, error);
			return (false);
		}
		token = token->next;
	}
	if (parenthesis == true && is_valid_grouping(toklist, error))
		return (true);
	return (false);
}

t_dlst	*extract_grouping_content(t_bin_tree *tree_node, t_dlst **toklist, t_error *error)
{
	if (!is_grouping(*toklist, error))
		return (NULL);
	tree_node->content->subshell = true;
	*toklist = (*toklist)->next;
	ft_dlstremove((*toklist)->prev, free_token_content, free);
	ft_dlstremove(ft_dlstlast(*toklist), free_token_content, free);
	return (*toklist);
}
