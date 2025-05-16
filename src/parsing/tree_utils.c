/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tree_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: juportie <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/13 14:21:17 by juportie          #+#    #+#             */
/*   Updated: 2025/05/15 11:11:38 by juportie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parsing.h"

t_bin_tree	*alloc_tree_node(void)
{
	t_bin_tree	*node;
	t_node_content	*content;

	node = ft_calloc(1, sizeof(t_bin_tree));
	if (node == NULL)
		return (NULL);
	content = ft_calloc(1, sizeof(t_node_content));
	if (content == NULL)
	{
		free(node);
		return (NULL);
	}
	node->content = content;
	return (node);
}

t_bin_tree	*tree_root(t_bin_tree *node)
{
	if (node == NULL)
		return (NULL);
	while (node->parent)
		node = node->parent;
	return (node);
}
