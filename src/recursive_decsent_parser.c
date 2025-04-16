/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   recursive_decsent_parser.c                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kura <kura@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/15 06:55:11 by ecasalin          #+#    #+#             */
/*   Updated: 2025/04/16 21:45:31 by kura             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <unistd.h>

#define PIPE 1
#define OR 2
#define AND 3
#define CHILD 0

typedef	struct s_cmd
{
	char	**cmd;
	char	*input;
	char	*output;
}			t_cmd;

typedef	struct s_node
{
	struct s_node	*prev;
	struct s_node	*left;
	struct s_node	*right;
	char			op;
	t_cmd			cmd;
}				t_node;

typedef	struct s_pipes_data
{
	int		pipe1[2];
	int		pipe2[2];
	char	to_close;
	char	is_init;
}				t_pipes_data;

typedef	struct s_var_data
{
	char	**env;
	int		last_cmd_ext_code;
}				t_var_data;

int	open_pipes(t_pipes_data *pipes)
{
	if (pipes->pipe1[0] == -1 && pipes->pipe2[0] == -1)
		return (pipe(pipes->pipe1));
	else if (pipes->pipe1[0] != -1)
	{
		pipes->to_close = 1;
		return (pipe(pipes->pipe1));
	}
	else
	{
		pipes->to_close = 2;
		return (pipe(pipes->pipe2));
	}
}

int	close_pipe(int *pipe[2])
{
	int	close_return;

	close_return = 0;
	close_return = close(*pipe[0]);
	close_return = close(*pipe[1]);
	*pipe[0] = -1;
	*pipe[1] = -1;
	if (close_return)
		perror("close_pipe");
	return (close_return);
}

int	reset_pipes(t_pipes_data *pipes)
{
	if (pipes->to_close == 1)
		return (close_pipe(&pipes->pipe1));
	if (pipes->to_close == 2)
		return (close_pipe(&pipes->pipe2));
	return (0);
}

/*while exec cmd 2 check if prev->prev->op is | : if it is, we good, if not, need to redirect cmd to stdout and set all pipes to -1 AND wait for all childs and return*/
/*if curr.op is | and prev is NULL, or perv.op is &&/||, we need to create a new process for the pipeline coming */

int	prepare_exec_left(t_pipes_data *pipes, t_node *curr_node, t_var_data *vars)
{
	int	child_pid;

	if (curr_node->left)
		return (recursive_descent_parser(curr_node->left, vars));
	if (curr_node->prev && curr_node->prev->op == PIPE)
		open_pipes(pipes);
	child_pid = fork();
	if (child_pid == CHILD)
	{
		link_pipes();
		set_redirections();
		exec();
	}

}

int	prepare_exec_right(t_pipes_data *pipes, t_node *curr_node, t_var_data *vars)
{
	if (curr_node->right)
		vars->last_cmd_ext_code = recursive_descent_parser(curr_node->right, vars);
	else
		return (exec(vars->last_cmd_ext_code, &pipes, curr_node));
}

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

int	recursive_descent_parser(t_node *curr_node, t_var_data *vars)
{
	static t_pipes_data	pipes;

	if (!pipes.is_init)
		pipes = (t_pipes_data){{-1, -1}, {-1, -1}, 0, 1};
	vars->last_cmd_ext_code = prepare_exec_left(&pipes, curr_node, vars);
	if (check_right_cmd_conditions(curr_node, vars->last_cmd_ext_code))
		return (prepare_exec_right(&pipes, curr_node, vars));
	return (vars->last_cmd_ext_code);
}
