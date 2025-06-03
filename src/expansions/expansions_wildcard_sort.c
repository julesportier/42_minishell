/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expansions_wildcard_sort.c                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: juportie <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/05 14:17:02 by juportie          #+#    #+#             */
/*   Updated: 2025/06/05 14:53:43 by juportie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../libft/src/libft.h"
#include "../parsing/parsing.h"
#include <stdio.h>

static t_dlst	*go_to_node(t_dlst *list, int node)
{
	while (node--)
	{
		if (!list->next)
			return (NULL);
		list = list->next;
	}
	return (list);
}

static t_bool	is_smaller(t_dlst *node_a, t_dlst *node_b)
{
	char	*node_a_str;
	char	*node_b_str;
	size_t	node_a_len;
	size_t	node_b_len;
	size_t	len;

	node_a_str = get_toklist_str(node_a);
	node_b_str = get_toklist_str(node_b);
	node_a_len = ft_strlen(node_a_str);
	node_b_len = ft_strlen(node_b_str);
	if (node_a_len < node_b_len)
		len = node_a_len;
	else
		len = node_b_len;
	if (ft_strncmp(node_a_str, node_b_str, len) < 0)
		return (true);
	else
		return (false);
}

static void	remove_token(t_dlst **toklist, t_dlst *token)
{
	if (token->prev == NULL)
		*toklist = (*toklist)->next;
	ft_dlstremove(token, NULL, NULL);
}

static void	move_node_to_pos(t_dlst **cwd_content_list, t_dlst *node, int pos)
{
	t_dlst	*smaller_node;

	if (node != *cwd_content_list)
	{
		remove_token(cwd_content_list, node);
		smaller_node = go_to_node(*cwd_content_list, pos);
		ft_dlstinsert_next(&smaller_node, node);
	}
}

void	sort_list(t_dlst **cwd_content_list)
{
	t_dlst	*smaller;
	t_dlst	*node;
	int	sorted;
	int	size;

	size = ft_dlstsize(*cwd_content_list);
	sorted = 0;
	while (sorted < size)
	{
		node = go_to_node(*cwd_content_list, sorted);
		smaller = node;
		while (node)
		{
			if (is_smaller(node, smaller))
				smaller = node;
			node = node->next;
		}
		move_node_to_pos(cwd_content_list, smaller, sorted);
		if (DEBUG)
		{
			printf("sorted list %d\n", sorted);
			print_toklist(*cwd_content_list, 0);
		}
		++sorted;
	}
}
