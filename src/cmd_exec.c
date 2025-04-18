/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd_exec.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ecasalin <ecasalin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/28 15:00:16 by ecasalin          #+#    #+#             */
/*   Updated: 2025/04/18 14:49:48 by ecasalin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	exec_cmd(char **array1, char **array2, int i, t_var_data *vars)
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

void	execution(t_node *curr_node, char **paths_array, t_var_data *vars)
{
	int	i;

	i = 0;
	if (paths_array != NULL && ft_strnstr(curr_node->cmd[0],
			"/", ft_nstrlen(curr_node->cmd[0])) == NULL)
	{
		while (paths_array != NULL && paths_array[i] != NULL)
		{
			paths_array[i] = ft_fstrjoin(paths_array[i],
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
