/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tree_build_grouping.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: juportie <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/06 09:44:07 by juportie          #+#    #+#             */
/*   Updated: 2025/05/20 11:39:17 by juportie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>
#include "../../libft/src/libft.h"
#include "../minishell.h"
#include "parsing.h"
#include "tree_build.h"

static t_bool	contains_parenthesis(t_dlst *toklist)
{
	while (toklist)
	{
		if (is_grouping_op(get_toklist_type(toklist)))
			return (true);
		toklist = toklist->next;
	}
	return (false);
}

static t_bool	is_correct_grouping(t_dlst *toklist, t_error *error)
{
	t_dlst	*last;

	last = ft_dlstlast(toklist);
	if (get_toklist_type(toklist) == left_parenthesis
		&& get_toklist_type(last) == right_parenthesis
		&& toklist->next != last)
	{
		return (true);
	}
	else
	{
		if (get_toklist_type(toklist) == left_parenthesis
		&& get_toklist_type(last) == right_parenthesis
		&& toklist->next == last)
		{
			print_syntax_error(
				"there must be something inside parenthesis ; unexpected token ",
				get_toklist_type(last), recoverable);
			*error = recoverable;
		}
		else if (contains_parenthesis(toklist))
		{
			print_syntax_error(
				"there can only be binary operators or redirections around parenthesis ; \
				unexpected token ",
				get_toklist_type(toklist), recoverable);
			*error = recoverable;
		}
		return (false);
	}
}

t_dlst	*extract_grouping_content(t_bin_tree *tree_node, t_dlst **toklist, t_error *error)
{
	int	nesting_level;
	t_dlst	*token;

	if (!is_correct_grouping(*toklist, error))
		return (NULL);
	tree_node->content->subshell = true;
	token = *toklist;
	*toklist = (*toklist)->next;
	ft_dlstremove(token, free_token_content, free);
	nesting_level = 1;
	token = *toklist;
	while (1)
	{
		nesting_level = update_parenthesis_nesting_level(token, nesting_level);
		if (check_nesting_level(nesting_level, token) != success)
		{
			print_syntax_error("unexpected token ", get_toklist_type(token), recoverable);
			*error = recoverable;
			return (NULL);
		}
		if (token->next)
			token = token->next;
		else
			break ;
	}
	ft_dlstremove(token, free_token_content, free);
	return (*toklist);
}
