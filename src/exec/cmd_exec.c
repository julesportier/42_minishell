/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd_exec.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kura <kura@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/28 15:00:16 by ecasalin          #+#    #+#             */
/*   Updated: 2025/04/26 11:58:02 by kura             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../libft/src/libft.h"
#include <unistd.h>
#include <stdio.h>
#include <sys/wait.h>
#include "../general_utils/utils.h"
#include "../error_handling/errors.h"
#include "../cleaning_utils/cleaning.h"
#include "exec.h"
#include "../minishell.h"

// int	execution(t_node *curr_node, char **paths_array, t_shell_vars *vars)
// {
// 	// expand_args(curr_node->cmd.cmd, vars);
// 	// expand_redirections(curr_node->cmd.input, curr_node->cmd.output, vars);
// 	// set_redirections(curr_node->cmd.input, curr_node->cmd.output);
// }

int	prepare_to_exec(t_node *curr_node, t_shell_vars *vars)
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

static void	exec_cmd(char **array1, char **array2, int i, t_shell_vars *vars)
{
	execve(array1[i], array2, vars->env);
	// if (errno == ENOEXEC || errno == EACCES)
	// 	exit_free_close(array1[i], 126, data);
	// if (errno == ENOENT)
	// 	exit_free_close(array1[i], 127, data);
	// else
	// 	exit_free_close(array1[i], 1, data);
	perror("EXEC_FAIL");
}

// static void	set_cmd_arg_array(t_data *data)
// {
// 	if (data->argv[data->cmd][0] == '\0' || data->argv[data->cmd][0] == ' ')
// 	{
// 		print_cmd_nfound(data);
// 		exit_free_close(NULL, 127, data);
// 	}
// 	data->cmd_arg_array = ft_split(data->argv[data->cmd], ' ');
// 	if (data->cmd_arg_array == NULL)
// 		exit_free_close("set_cmd_arg_array, malloc error", 1, data);
// }

void	execution(t_node *curr_node, char **paths_array, t_shell_vars *vars)
{
	int	i;

	i = 0;
	if (paths_array != NULL && ft_strnstr(curr_node->cmd[0],
			"/", ft_strlen(curr_node->cmd[0])) == NULL)
	{
		while (paths_array != NULL && paths_array[i] != NULL)
		{
			paths_array[i] = free_strjoin(paths_array[i],
					curr_node->cmd[0]);
			// if (paths_array[i] == NULL)
			// 	exit_free_close("check_cmd_path, malloc error", 1, data);
			if (access(paths_array[i], F_OK) == 0)
				exec_cmd(paths_array, curr_node->cmd, i, vars);
			i++;
		}
		perror("BAD PATH");
		// if (print_cmd_nfound(data) == 0)
		// 	exit_free_close(NULL, 1, data);
		// ft_putstr_fd(data->cmd_arg_array[0], 2);
		// exit_free_close(NULL, 127, data);
	}
	exec_cmd(curr_node->cmd, curr_node->cmd, 0, vars);
}
