/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   build_tree_redirections.c                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: juportie <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/20 11:13:48 by juportie          #+#    #+#             */
/*   Updated: 2025/05/20 11:14:36 by juportie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../libft/src/libft.h"
#include "../minishell.h"
#include "parsing.h"
#include "build_tree.h"

static void	move_redir_token(t_dlst **target, t_dlst **source_head, t_dlst **source)
{
	t_dlst	*node;

	node = *source;
	if ((*source)->prev == NULL)
		*source_head = (*source_head)->next;
	*source = (*source)->next;
	ft_dlstremove(node, NULL, NULL);
	ft_dlstadd_back(target, node);
}

static t_error	set_redir_couple(t_bin_tree *tree_node, t_dlst **toklist, t_dlst **token)
{
	t_dlst	**target;

	if ((*token)->next == NULL
		|| is_primary(get_toklist_type((*token)->next)) == false)
	{
		return (print_syntax_error(
			"illformed redirection, unexpected ", get_toklist_type(*token), recoverable));
	}
	else if (get_toklist_type(*token) == redir_output
		|| get_toklist_type(*token) == append_output)
	{
		target = &(tree_node->content->outputs);
	}
	else
	{
		target = &(tree_node->content->inputs);
	}
	move_redir_token(target, toklist, token);
	move_redir_token(target, toklist, token);
	return (success);
}

t_error	set_redirection(t_bin_tree *tree_node, t_dlst **toklist)
{
	int	nesting_level;
	t_dlst	*token;

	nesting_level = 0;
	token = *toklist;
	while (token)
	{
		nesting_level = update_parenthesis_nesting_level(token, nesting_level);
		if (is_redir_op(get_toklist_type(token)) && nesting_level == 0)
		{
			if (set_redir_couple(tree_node, toklist, &token) != success)
				return (recoverable);
		}
		else
			token = token->next;
	}
	return (success);
}
