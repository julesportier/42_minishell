/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_command_tree.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ecasalin <ecasalin@42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/15 06:55:11 by ecasalin          #+#    #+#             */
/*   Updated: 2025/06/01 15:26:23 by ecasalin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "exec.h"
#include "../signals_utils/signals_utils.h"
#include "../parsing/parsing.h"

static int	recurse_left_side(t_bin_tree *curr_node, t_shell_vars *vars, t_error *error)
{
	if (curr_node->left)
		return (exec_command_tree(curr_node->left, vars, error));
	else
		return (create_exec_setup(curr_node, vars, error));
}

static int	recurse_right_side(t_bin_tree *curr_node, t_shell_vars *vars, t_error *error)
{
	if (curr_node->right)
		return (exec_command_tree(curr_node->right, vars, error));
	else
		return (create_exec_setup(curr_node, vars, error));
}

static int	check_right_cmd_conditions(t_bin_tree *curr_node, t_error *error, t_shell_vars *vars)
{
	if (*error == critical)
		return (0);
	if (g_sig != 0)
		return (0);
	if (!curr_node->right)
		return (0);
	if (curr_node->operator == or && vars->last_cmd_ext_code == SUCCESS)
		return (0);
	if (curr_node->operator == and && vars->last_cmd_ext_code != SUCCESS)
		return (0);
	return (1);
}

int	exec_command_tree(t_bin_tree *curr_node, t_shell_vars *vars, t_error *error)
{
	if (*error)
		return (ERROR);
	if (curr_node->operator == pipeline)
		return (fork_pipeline_sides(curr_node, vars, error));
	if (curr_node->content->subshell == true)
		vars->last_cmd_ext_code = fork_subshell(curr_node, vars, error);
	else
	{
		vars->last_cmd_ext_code = recurse_left_side(curr_node, vars, error);
		if (check_right_cmd_conditions(curr_node, error, vars))
			vars->last_cmd_ext_code = recurse_right_side(curr_node, vars, error);
	}
	return (vars->last_cmd_ext_code);
}
