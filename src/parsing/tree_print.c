/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tree_print.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: juportie <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/13 11:06:41 by juportie          #+#    #+#             */
/*   Updated: 2025/06/13 15:03:46 by juportie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>
#include "parsing.h"

static void	print_operator(t_bin_tree *node, int indent)
{
	const char	*yellow = "\e[0;33m";
	const char	*reset = "\e[0m";

	print_indent(indent);
	if (node->content->subshell)
	{
		printf(
			"%s%s (subshell)\n", yellow, token_type_to_str(node->operator));
		print_indent(indent);
		printf("inputs:\n");
		print_toklist(node->content->inputs, indent + 1);
		print_indent(indent);
		printf("outputs:\n");
		print_toklist(node->content->outputs, indent + 1);
		printf("%s", reset);
	}
	else
		printf("%s%s%s\n", yellow, token_type_to_str(node->operator), reset);
}

static void	print_commands(t_bin_tree *node, int indent)
{
	const char	*green = "\e[0;32m";
	const char	*reset = "\e[0m";

	print_indent(indent);
	if (node->content->subshell)
		printf("%sleaf (subshell)%s\n", green, reset);
	else
		printf("%sleaf%s\n", green, reset);
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

static void	print_tree_node(t_bin_tree *node, int indent)
{
	if (node->operator)
		print_operator(node, indent);
	else
		print_commands(node, indent);
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
