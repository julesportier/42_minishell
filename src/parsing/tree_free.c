/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tree_free.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: juportie <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/13 12:25:12 by juportie          #+#    #+#             */
/*   Updated: 2025/05/13 17:26:34 by juportie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parsing.h"

static void	free_tree_node(t_bin_tree **node)
{
	free_toklist(&((*node)->content->inputs));
	free_toklist(&((*node)->content->outputs));
	free_toklist(&((*node)->content->tokens_list));
	free((*node)->content);
	(*node)->content = NULL;
	free(*node);
	*node = NULL;
}

void	free_tree(t_bin_tree **tree)
{
	t_bin_tree	**temp;

	if (*tree == NULL)
		return ;
	temp = tree;
	if ((*tree)->left)
		free_tree(&((*tree)->left));
	if ((*tree)->right)
		free_tree(&((*tree)->right));
	free_tree_node(temp);
}
