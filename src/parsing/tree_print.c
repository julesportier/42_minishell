/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tree_print.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: juportie <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/13 11:06:41 by juportie          #+#    #+#             */
/*   Updated: 2025/05/13 16:11:23 by juportie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>
#include "parsing.h"

static void	print_tree_node(t_bin_tree *node, int indent)
{
	if (node->operator)
	{
		print_indent(indent);
		printf("%s\n", token_type_to_str(node->operator));
	}
	else
	{
		print_indent(indent);
		printf("leaf\n");
		print_indent(indent + 1);
		printf("inputs:\n");
		print_toklist(node->content->inputs, indent + 2);
		print_indent(indent + 1);
		printf("outputs:\n");
		print_toklist(node->content->outputs, indent + 2);
		print_indent(indent + 1);
		printf("commands:\n");
		print_toklist(node->content->tokens_list, indent + 2);
	}
}

void	print_tree(t_bin_tree *tree, int level)
{
	if (tree)
	{
		print_tree_node(tree, level);
		print_tree(tree->right, level + 1);
		print_tree(tree->left, level + 1);
	}
}
