/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipeline_handling_1.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kura <kura@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/21 14:19:24 by ecasalin          #+#    #+#             */
/*   Updated: 2025/04/26 11:58:02 by kura             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

// #include "../cleaning_utils/cleaning.h"
#include "../error_handling/errors.h"
#include <sys/wait.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include "exec.h"

static int	link_pipe_to_stdout(int *pip)
{
	if (dup2(pip[WRITE], STDOUT_FILENO) == FAILURE)
	{
		perror("INTERNAL ERROR");
		close_pipe(pip);
		exit (ERROR);
	}
	close_pipe(pip);
	return (SUCCESS);
}

static int	link_pipe_to_stdin(int *pip)
{
	if (dup2(pip[READ], STDIN_FILENO) == FAILURE)
	{
		perror("INTERNAL ERROR");
		close_pipe(pip);
		exit (ERROR);
	}
	close_pipe(pip);
	return (SUCCESS);
}

static void	continue_pipeline_left_process(t_node *curr_node, int *pip, t_shell_vars *vars)
{
	link_pipe_to_stdout(pip);
	if (curr_node->left)
	{
		vars->last_cmd_ext_code = (exec_cmd_tree(curr_node->left, vars));
		exit(vars->last_cmd_ext_code);
	}
	else
		prepare_to_exec(curr_node, vars);
}

static void	continue_pipeline_right_process(t_node *curr_node, int *pip, t_shell_vars *vars)
{
	link_pipe_to_stdin(pip);
	if (curr_node->right)
	{
		vars->last_cmd_ext_code = (exec_cmd_tree(curr_node->right, vars));
		exit(vars->last_cmd_ext_code);
	}
	else
		prepare_to_exec(curr_node, vars);
}

int	fork_pipeline_sides(t_node *curr_node, t_shell_vars *vars)
{
	int	pip[2];
	int	child_pid;

	if (pipe(pip) == FAILURE)
		return (return_perror("INTERNAL ERROR", ERROR));
	child_pid = fork();
	if (child_pid == FAILURE)
		return (close_pipe_perror("INTERNAL ERROR", ERROR, pip));
	if (child_pid == CHILD)
		continue_pipeline_left_process(curr_node, pip, vars);
	child_pid = fork();
	if (child_pid == FAILURE)
	{
		wait(NULL);
		return (close_pipe_perror("INTERNAL ERROR", ERROR, pip));
	}
	if (child_pid == CHILD)
		continue_pipeline_right_process(curr_node, pip, vars);
	if (close_pipe(pip) == FAILURE)
	{
		wait_childs(child_pid);
		return (return_perror("INTERNAL ERROR", ERROR));
	}
	vars->last_cmd_ext_code = wait_childs(child_pid);
	return (vars->last_cmd_ext_code);
}
