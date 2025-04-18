/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   recursive_decsent_parser.c                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ecasalin <ecasalin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/15 06:55:11 by ecasalin          #+#    #+#             */
/*   Updated: 2025/04/18 16:00:25 by ecasalin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// int	open_pipes(t_pipes_data *pipes)
// {
// 	if (pipes->pipe1[0] == -1 && pipes->pipe2[0] == -1)
// 		return (pipe(pipes->pipe1));
// 	else if (pipes->pipe1[0] != -1)
// 	{
// 		pipes->to_close = 1;
// 		return (pipe(pipes->pipe1));
// 	}
// 	else
// 	{
// 		pipes->to_close = 2;
// 		return (pipe(pipes->pipe2));
// 	}
// }

// int	close_pipe(int *pipe[2])
// {
// 	int	close_return;

// 	close_return = 0;
// 	close_return = close(*pipe[0]);
// 	close_return = close(*pipe[1]);
// 	*pipe[0] = -1;
// 	*pipe[1] = -1;
// 	if (close_return)
// 		perror("close_pipe");
// 	return (close_return);
// }

// int	reset_pipes(t_pipes_data *pipes)
// {
// 	if (pipes->to_close == 1)
// 		return (close_pipe(&pipes->pipe1));
// 	if (pipes->to_close == 2)
// 		return (close_pipe(&pipes->pipe2));
// 	return (0);
// }

/*while exec cmd 2 check if prev->prev->op is | : if it is, we good, if not, need to redirect cmd to stdout and set all pipes to -1 AND wait for all childs and return*/
/*if curr.op is | and prev is NULL, or perv.op is &&/||, we need to create a new process for the pipeline coming */

// int	execution(t_node *curr_node, char **paths_array, t_var_data *vars)
// {
// 	// expand_args(curr_node->cmd.cmd, vars);
// 	// expand_redirections(curr_node->cmd.input, curr_node->cmd.output, vars);
// 	// set_redirections(curr_node->cmd.input, curr_node->cmd.output);
	
// }

int	get_exit_code(int child_exit_status)
{
	if (!WIFEXITED(child_exit_status))
		return (128 + WTERMSIG(child_exit_status));
	return (WEXITSTATUS(child_exit_status));
}

int	wait_childs(int second_child_pid)
{
	int	child_exit_status;
	
	waitpid(second_child_pid, &child_exit_status, 0);
	waitpid(0, NULL, 0);
	return (get_exit_code(child_exit_status));
}

int	prepare_to_exec(t_node *curr_node, t_var_data *vars)
{
	char	**paths_array;
	int		child_pid;
	int		child_exit_status;

	paths_array = create_paths_array(vars);
	// if (is_builtin(curr_node->cmd))
		// return (exec_builtin(curr_node, vars));
	if (curr_node->prev && curr_node->prev->op == PIPE)
		execution(curr_node, paths_array, vars);
	child_pid = fork();
	if (child_pid == CHILD)
		execution(curr_node, paths_array, vars);
	else
	{
		waitpid(child_pid, &child_exit_status, 0);
		return (get_exit_code(child_exit_status));
	}
	return (ERROR);
}

// int	prepare_exec_right(t_pipes_data *pipes, t_node *curr_node, t_var_data *vars)
// {
// 	if (curr_node->right)
// 		vars->last_cmd_ext_code = recursive_exec(curr_node->right, vars);
// 	else
// 		return (exec(vars->last_cmd_ext_code, &pipes, curr_node));
// }

int	check_right_cmd_conditions(t_node *curr_node, int left_cmd_return)
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

int	link_pipe_to_stdout(int pip[2])
{
	if (dup2(pip[WRITE], STDOUT_FILENO))
	{
		close(pip[READ]);
		close(pip[WRITE]);
		return (ERROR);
	}
	close(pip[READ]);
	close(pip[WRITE]);
	return (SUCCESS);
}

int	link_pipe_to_stdin(int pip[2])
{
	if (dup2(pip[READ], STDIN_FILENO))
	{
		close(pip[READ]);
		close(pip[WRITE]);
		return (ERROR);
	}
	close(pip[READ]);
	close(pip[WRITE]);
	return (SUCCESS);
}

int	recursive_exec(t_node *curr_node, t_var_data *vars)
{
	int	pip[2];
	int	child_pid;
	
	if (curr_node->op == PIPE)
	{
		pipe(pip);
		child_pid = fork();
		if (child_pid == CHILD)
		{
			link_pipe_to_stdout(pip);
			if (curr_node->left)
				vars->last_cmd_ext_code = (recursive_exec(curr_node->left, vars));
			else
				prepare_to_exec(curr_node, vars);
		}
		if (child_pid != CHILD)
			child_pid = fork();
		if (child_pid == CHILD)
		{
			link_pipe_to_stdin(pip);
			if (curr_node->right)
				vars->last_cmd_ext_code = (recursive_exec(curr_node->right, vars));
			else
				prepare_to_exec(curr_node, vars);
		}
		if (child_pid != CHILD)
		{
			close(pip[READ]);
			close(pip[WRITE]);
			vars->last_cmd_ext_code = wait_childs(child_pid);
			return (vars->last_cmd_ext_code);
		}
	}
	if (curr_node->left)
		vars->last_cmd_ext_code = (recursive_exec(curr_node->left, vars));
	else
		vars->last_cmd_ext_code = prepare_to_exec(curr_node, vars);
	if (check_right_cmd_conditions(curr_node, vars->last_cmd_ext_code))
	{
		if (curr_node->right)
			vars->last_cmd_ext_code = (recursive_exec(curr_node->right, vars));
		else
			vars->last_cmd_ext_code = prepare_to_exec(curr_node, vars);
	}
	return (vars->last_cmd_ext_code);
}


// int	recursive_exec(t_node *curr_node, t_special_var *special_var)
// {
// 	static t_pipes_data	pipes;
// 	int					pipeline_pid;
// 	int					pipeline_exit;
	
// 	pipeline_pid = 0;
// 	if (!pipes.is_init)
// 		pipes = (t_pipes_data){{-1, -1}, {-1, -1}, 0, 1};
// 	if (curr_node->op == PIPE && !curr_node->prev || curr_node->prev->op != PIPE)
// 		pipeline_pid = fork();
// 	if (pipeline_pid != CHILD)
// 	{
// 		waitpid(pipeline_pid, &pipeline_exit, 0);
// 		special_var->last_cmd_ext_code = get_exit_status(pipeline_exit);
// 		return (special_var->last_cmd_ext_code);
// 	}
// 	special_var->last_cmd_ext_code = prepare_exec_left(&pipes, curr_node, special_var);
// 	if (check_right_cmd_conditions(curr_node, special_var->last_cmd_ext_code))
// 		return (prepare_exec_right(&pipes, curr_node, special_var));
// 	return (special_var->last_cmd_ext_code);
// }