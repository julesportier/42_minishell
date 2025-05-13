/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tree_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: juportie <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/13 14:21:17 by juportie          #+#    #+#             */
/*   Updated: 2025/05/13 14:26:30 by juportie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parsing.h"

t_bin_tree	*tree_root(t_bin_tree *node)
{
	if (node == NULL)
		return (NULL);
	while (node->parent)
		node = node->parent;
	return (node);
}
