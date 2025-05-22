/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   subshells_handling.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ecasalin <ecasalin@42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/20 21:13:38 by ecasalin          #+#    #+#             */
/*   Updated: 2025/05/22 11:02:14 by ecasalin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include "../minishell.h"
#include "../parsing/parsing.h" //rajouter ifndef dans tree_build.h
#include "../error_handling/errors.h"
#include "../exec/exec.h"
#include "../cleaning_utils/cleaning.h"

static void	continue_in_subshell(t_bin_tree *curr_node, t_shell_vars *vars, t_error *error)
{
	vars->last_cmd_ext_code = exec_cmd_tree(curr_node, vars, error);
	free_tree_and_vars(tree_root(curr_node), vars);
	exit(vars->last_cmd_ext_code);
}

int	fork_subshell(t_bin_tree *curr_node, t_shell_vars *vars, t_error *error)
{
	pid_t	pid;
	int		exit_value;

	curr_node->content->subshell = false;
	pid = fork();
	if (pid == FAILURE)
		return (return_perror_set_err("minishell: execution: fork error", error, recoverable));
	if (pid == CHILD)
	{
		exit_value = set_io_fds(curr_node);
		if (exit_value != SUCCESS)
		{
			free_tree_and_vars(tree_root(curr_node), vars);
			exit(exit_value);
		}
		continue_in_subshell(curr_node, vars, error);
	}
	return (wait_child());
}

