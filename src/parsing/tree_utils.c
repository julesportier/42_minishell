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

t_bin_tree	*alloc_tree_node(t_error *error)
{
	t_bin_tree	*node;
	t_node_content	*content;

	node = ft_calloc(1, sizeof(t_bin_tree));
	if (node == NULL)
		return (null_perror_alloc(critical, error));
	content = ft_calloc(1, sizeof(t_node_content));
	if (content == NULL)
	{
		free(node);
		return (null_print_alloc(critical, error));
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
