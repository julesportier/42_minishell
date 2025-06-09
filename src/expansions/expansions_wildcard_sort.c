/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expansions_wildcard_sort.c                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: juportie <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/05 14:17:02 by juportie          #+#    #+#             */
/*   Updated: 2025/06/07 10:31:22 by juportie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../libft/src/libft.h"
#include "expansions.h"

static t_dlst	*go_to_node(t_dlst *list, int pos)
{
	while (pos-- && list)
		list = list->next;
	return (list);
}

static void	remove_token(t_dlst **toklist, t_dlst *token)
{
	if (token->prev == NULL)
		*toklist = (*toklist)->next;
	ft_dlstremove(token, NULL, NULL);
}

static void	move_node_to_pos(
	t_dlst **cwd_content_list,
	t_dlst *node,
	int pos)
{
	t_dlst	*smaller_node;

	if (node != *cwd_content_list)
	{
		remove_token(cwd_content_list, node);
		if (pos == 0)
		{
			ft_dlstadd_front(cwd_content_list, node);
		}
		else
		{
			smaller_node = go_to_node(*cwd_content_list, pos - 1);
			ft_dlstinsert_next(&smaller_node, node);
		}
	}
}

void	sort_list(t_dlst **cwd_content_list)
{
	t_dlst	*smaller;
	t_dlst	*node;
	int		sorted_pos;
	int		size;

	size = ft_dlstsize(*cwd_content_list);
	sorted_pos = 0;
	while (sorted_pos < size)
	{
		node = go_to_node(*cwd_content_list, sorted_pos);
		smaller = node;
		while (node)
		{
			if (is_smaller(node, smaller))
				smaller = node;
			node = node->next;
		}
		if (smaller != go_to_node(*cwd_content_list, sorted_pos))
			move_node_to_pos(cwd_content_list, smaller, sorted_pos);
		++sorted_pos;
	}
}
