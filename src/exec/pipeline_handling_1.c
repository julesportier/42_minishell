/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipeline_handling_1.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ecasalin <ecasalin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/21 14:19:24 by ecasalin          #+#    #+#             */
/*   Updated: 2025/05/19 10:55:47 by ecasalin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

// #include "../cleaning_utils/cleaning.h"
#include "../error_handling/errors.h"
#include "../parsing/parsing.h"
#include "../cleaning_utils/cleaning.h"
#include <sys/wait.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include "exec.h"

static int	link_pipe_to_stdout(int *pip)
{
	if (dup2(pip[WRITE], STDOUT_FILENO) == FAILURE)
		return (close_pipe_perror("minishell: execution: dup2 error", ERROR, pip));
	close_pipe(pip);
	return (SUCCESS);
}

static int	link_pipe_to_stdin(int *pip)
{
	if (dup2(pip[READ], STDIN_FILENO) == FAILURE)
		return (close_pipe_perror("minishell: execution: dup2 error", ERROR, pip));
	close_pipe(pip);
	return (SUCCESS);
}

static void	continue_pipeline_left_process(t_bin_tree *curr_node, int *pip, t_shell_vars *vars, t_error *error)
{
	if (link_pipe_to_stdout(pip) == ERROR)
	{
		free_tree_and_vars(tree_root(curr_node), vars);
		exit (ERROR);
	}
	if (curr_node->left)
	{
		vars->last_cmd_ext_code = (exec_cmd_tree(curr_node->left, vars, error));
		free_tree_and_vars(tree_root(curr_node), vars);
		exit(vars->last_cmd_ext_code);
	}
	else
		prepare_to_exec(curr_node, vars, error);
}

static void	continue_pipeline_right_process(t_bin_tree *curr_node, int *pip, t_shell_vars *vars, t_error *error)
{
	if (link_pipe_to_stdin(pip) == ERROR)
	{
		free_tree_and_vars(tree_root(curr_node), vars);
		exit (ERROR);
	}
	if (curr_node->right)
	{
		vars->last_cmd_ext_code = (exec_cmd_tree(curr_node->right, vars, error));
		free_tree_and_vars(tree_root(curr_node), vars);
		exit(vars->last_cmd_ext_code);
	}
	else
		prepare_to_exec(curr_node, vars, error);
}

int	fork_pipeline_sides(t_bin_tree *curr_node, t_shell_vars *vars, t_error *error)
{
	int	pip[2];
	int	child_pid;

	if (pipe(pip) == FAILURE)
		return (return_perror("minishell: execution: pipe error", ERROR));
	child_pid = fork();
	if (child_pid == FAILURE)
		return (close_pipe_perror("minishell: execution: fork error", ERROR, pip));
	if (child_pid == CHILD)
		continue_pipeline_left_process(curr_node, pip, vars, error);
	child_pid = fork();
	if (child_pid == FAILURE)
	{
		wait(NULL);
		return (close_pipe_perror("minishell: execution: fork error", ERROR, pip));
	}
	if (child_pid == CHILD)
		continue_pipeline_right_process(curr_node, pip, vars, error);
	if (close_pipe(pip) == ERROR)
	{
		wait_childs(child_pid);
		return (ERROR);
	}
	vars->last_cmd_ext_code = wait_childs(child_pid);
	return (vars->last_cmd_ext_code);
}
