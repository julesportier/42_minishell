/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tree_build_utils.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: juportie <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/06 09:44:07 by juportie          #+#    #+#             */
/*   Updated: 2025/06/13 14:58:21 by juportie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>
#include "../../libft/src/libft.h"
#include "../minishell.h"
#include "parsing.h"

t_error	free_tree_materials(
	t_dlst **toklist,
	t_bin_tree **tree_node,
	t_error *error)
{
	if (toklist)
	{
		free_toklist(toklist);
		*toklist = NULL;
	}
	if (tree_node && *tree_node)
	{
		free_tree(tree_node);
		*tree_node = NULL;
	}
	return (*error);
}

t_error	alloc_tree_childs_nodes(t_bin_tree **tree_node, t_error *error)
{
	(*tree_node)->left = alloc_tree_node(error);
	if (*error)
		return (*error);
	(*tree_node)->right = alloc_tree_node(error);
	if (*error)
		return (*error);
	(*tree_node)->left->parent = *tree_node;
	(*tree_node)->right->parent = *tree_node;
	return (success);
}
