/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipeline_handling.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ecasalin <ecasalin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/21 14:19:24 by ecasalin          #+#    #+#             */
/*   Updated: 2025/06/18 13:11:53 by ecasalin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../error_handling/errors.h"
#include "../parsing/parsing.h"
#include "../cleaning_utils/cleaning.h"
#include <sys/wait.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include "exec.h"
#include "../input/input.h"
#include "../expansions/expansions.h"

static int	link_pipe_to_stdout(int *pip)
{
	if (dup2(pip[WRITE], STDOUT_FILENO) == FAILURE)
		return (close_pipe_perror("minishell: execution: "
				"dup2 error", ERROR, pip));
	close_pipe(pip);
	return (SUCCESS);
}

static int	link_pipe_to_stdin(int *pip)
{
	if (dup2(pip[READ], STDIN_FILENO) == FAILURE)
		return (close_pipe_perror("minishell: execution: "
				"dup2 error", ERROR, pip));
	close_pipe(pip);
	return (SUCCESS);
}

static void	continue_pipeline_left_process(t_bin_tree *curr_node,
			int *pip, t_shell_vars *vars, t_error *error)
{
	if (curr_node->content->subshell == true)
	{
		expand_toklist(&curr_node->content->inputs, vars, error);
		if (*error == success
			&& check_redir_list(curr_node->content->inputs, error) == ERROR)
			ft_putstr_fd("minishell: syntax error: ill-formed input\n", 2);
		if (*error == success && set_input(curr_node, error) == ERROR)
			*error = recoverable;
	}
	if (*error || link_pipe_to_stdout(pip) == ERROR)
	{
		free_tree_and_vars(tree_root(curr_node), vars);
		exit (ERROR);
	}
	if (curr_node->left)
	{
		vars->last_cmd_ext_code = exec_command_tree(curr_node->left,
				vars, error);
		free_tree_and_vars(tree_root(curr_node), vars);
		exit(vars->last_cmd_ext_code);
	}
	else
		create_exec_setup(curr_node, vars, error);
}

static void	continue_pipeline_right_process(t_bin_tree *curr_node,
			int *pip, t_shell_vars *vars, t_error *error)
{
	if (curr_node->content->subshell == true)
	{
		expand_toklist(&curr_node->content->outputs, vars, error);
		if (*error == success
			&& check_redir_list(curr_node->content->outputs, error) == ERROR)
			ft_putstr_fd("minishell: syntax error: ill-formed output\n", 2);
		if (*error == success && set_output(curr_node, error) == ERROR)
			*error = recoverable;
	}
	if (*error || link_pipe_to_stdin(pip) == ERROR)
	{
		free_tree_and_vars(tree_root(curr_node), vars);
		exit(ERROR);
	}
	if (curr_node->right)
	{
		vars->last_cmd_ext_code = exec_command_tree(curr_node->right,
				vars, error);
		free_tree_and_vars(tree_root(curr_node), vars);
		exit(vars->last_cmd_ext_code);
	}
	else
		create_exec_setup(curr_node, vars, error);
}

int	fork_pipeline_sides(t_bin_tree *curr_node,
		t_shell_vars *vars, t_error *error)
{
	int		pip[2];
	pid_t	child_pid;

	if (pipe(pip) == FAILURE)
		return (perror_set_err("minishell: execution: "
				"pipe error", error, recoverable));
	child_pid = fork();
	if (child_pid == FAILURE)
		return (return_close_pipe_perror("minishell: execution: "
				"fork error", pip, error, recoverable));
	if (child_pid == CHILD)
		continue_pipeline_left_process(curr_node, pip, vars, error);
	child_pid = fork();
	if (child_pid == FAILURE)
		return (wait_close_pipe_perror("minishell: execution: "
				"fork error", pip, error, recoverable));
	if (child_pid == CHILD)
		continue_pipeline_right_process(curr_node, pip, vars, error);
	if (close_pipe(pip) == ERROR)
	{
		wait_2_children(child_pid);
		return (ERROR);
	}
	vars->last_cmd_ext_code = wait_2_children(child_pid);
	return (vars->last_cmd_ext_code);
}
