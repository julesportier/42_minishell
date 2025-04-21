/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_cmd_tree.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ecasalin <ecasalin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/15 06:55:11 by ecasalin          #+#    #+#             */
/*   Updated: 2025/04/21 16:36:24 by ecasalin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	recurse_left_side(t_node *curr_node, t_var_data *vars)
{
	if (curr_node->left)
		return (exec_cmd_tree(curr_node->left, vars));
	else
		return (prepare_to_exec(curr_node, vars));
}

static int	recurse_right_side(t_node *curr_node, t_var_data *vars)
{
	if (curr_node->right)
		return (exec_cmd_tree(curr_node->right, vars));
	else
		return (prepare_to_exec(curr_node, vars));
}

static int	check_right_cmd_conditions(t_node *curr_node, int left_cmd_return)
{
	if (!curr_node->right)
		return (0);
	if (curr_node->op == OR)
		if (left_cmd_return == 0)
			return (0);
	if (curr_node->op == AND)
		if (left_cmd_return)
			return (0);
	return (1);
}

int	exec_cmd_tree(t_node *curr_node, t_var_data *vars)
{
	if (curr_node->op == PIPE)
		return (fork_pipeline_sides(curr_node, vars));
	else
	{
		vars->last_cmd_ext_code = recurse_left_side(curr_node, vars);
		if (check_right_cmd_conditions(curr_node, vars->last_cmd_ext_code))
			vars->last_cmd_ext_code = recurse_right_side(curr_node, vars);
		return (vars->last_cmd_ext_code);
	}
} //en sortie finale on detruit l'arbre
