/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tree_build.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: juportie <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/06 09:44:07 by juportie          #+#    #+#             */
/*   Updated: 2025/06/13 14:58:53 by juportie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>
#include "../../libft/src/libft.h"
#include "../minishell.h"
#include "parsing.h"
#include "tree_build.h"

static t_dlst	*find_operator(
	t_bin_tree *tree_node,
	t_dlst **toklist,
	t_error *error)
{
	t_dlst	*token;

	if (*toklist == NULL)
	{
		*error = recoverable;
		ft_putstr_fd("minishell: syntax error: ill-formed input\n", 2);
		return (NULL);
	}
	token = find_control_op(*toklist, error);
	if (token || *error)
		return (token);
	token = find_pipeline_op(*toklist, error);
	if (token || *error)
		return (token);
	*error = set_redirection(tree_node, toklist);
	if (*error || *toklist == NULL)
		return (NULL);
	token = extract_grouping_content(tree_node, toklist, error);
	if (*error || token == NULL)
		return (NULL);
	token = find_operator(tree_node, toklist, error);
	if (token || *error)
		return (token);
	return (NULL);
}

static t_bool	set_pivot(
	t_dlst *pivot,
	t_bin_tree **tree_node,
	t_dlst **toklist)
{
	if (pivot)
	{
		(*tree_node)->operator = get_toklist_type(pivot);
		return (true);
	}
	else
	{
		(*tree_node)->operator = null;
		(*tree_node)->content->tokens_list = *toklist;
		return (false);
	}
}

static t_error	populate_parse_tree(
	t_bin_tree **tree_node,
	t_dlst **toklist,
	t_error *error)
{
	t_dlst	*toklist_left;
	t_dlst	*toklist_right;
	t_dlst	*pivot;

	pivot = find_operator(*tree_node, toklist, error);
	if (*error)
		return (free_tree_materials(toklist, tree_node, error));
	else if (!set_pivot(pivot, tree_node, toklist))
		return (success);
	if (alloc_tree_childs_nodes(tree_node, error) != success)
		return (free_tree_materials(toklist, tree_node, error));
	if (divide_tokens_list(
			&toklist_left, &toklist_right, toklist, &pivot) != success)
	{
		*error = recoverable;
		free_tree_materials(NULL, tree_node, error);
		return (*error);
	}
	populate_parse_tree(&(*tree_node)->left, &toklist_left, error);
	populate_parse_tree(&(*tree_node)->right, &toklist_right, error);
	if (*error)
		return (free_tree_materials(NULL, tree_node, error));
	return (success);
}

t_bin_tree	*build_parse_tree(t_dlst **toklist, t_error *error)
{
	t_bin_tree	*parse_tree;

	parse_tree = alloc_tree_node(error);
	if (*error)
	{
		free_toklist(toklist);
		return (NULL);
	}
	if (populate_parse_tree(&parse_tree, toklist, error) != SUCCESS)
		return (NULL);
	else
		return (parse_tree);
}
