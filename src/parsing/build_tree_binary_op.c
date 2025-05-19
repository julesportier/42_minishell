/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   build_tree_binary_op.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: juportie <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/15 11:14:12 by juportie          #+#    #+#             */
/*   Updated: 2025/05/15 11:17:11 by juportie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"
#include "parsing.h"
#include "build_tree.h"

static t_dlst	*find_binary_op(
	t_dlst *toklist,
	t_bool fptr_is_operator(enum e_token_type))
//	t_error *error)
{
	int	nesting_level;
	t_dlst	*operator;

	nesting_level = 0;
	operator = NULL;
	while (toklist)
	{
		nesting_level = update_parenthesis_nesting_level(toklist, nesting_level);
		if (fptr_is_operator(get_toklist_type(toklist)) && nesting_level == 0)
			operator = toklist;
		toklist = toklist->next;
	}
	return (operator);
}

t_dlst	*find_control_op(t_dlst *toklist)//, t_error *error)
{
	return (find_binary_op(toklist, is_control_op));//, error));
}

t_dlst	*find_pipeline_op(t_dlst *toklist)//, t_error *error)
{
	return (find_binary_op(toklist, is_pipeline_op));//, error));
}

int	divide_tokens_list(
		t_dlst	**toklist_left,
		t_dlst 	**toklist_right,
		t_dlst	**toklist,
		t_dlst	**pivot)
{
	if (*toklist == *pivot)
	{
		print_syntax_error("missing operand before ", get_toklist_type(*pivot), ERROR);
		free_toklist(toklist);
		return (ERROR);
	}
	if (ft_dlstlast(*toklist) == *pivot)
	{
		print_syntax_error("missing operand after ", get_toklist_type(*pivot), ERROR);
		free_toklist(toklist);
		return (ERROR);
	}
	*toklist_right = ((*pivot)->next);
	(*toklist_right)->prev = NULL;
	ft_dlstsplit(*pivot);
	free_token_content((*pivot)->content);
	free(*pivot);
	*pivot = NULL;
	*toklist_left = *toklist;
	return (SUCCESS);
}
