/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd_exec.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ecasalin <ecasalin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/28 15:00:16 by ecasalin          #+#    #+#             */
/*   Updated: 2025/04/18 08:58:44 by ecasalin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	exec_cmd(char **array1, char **array2, int i, t_data *data)
{
	execve(array1[i], array2, data->envp);
	if (errno == ENOEXEC || errno == EACCES)
		exit_free_close(array1[i], 126, data);
	if (errno == ENOENT)
		exit_free_close(array1[i], 127, data);
	else
		exit_free_close(array1[i], 1, data);
}

static void	set_cmd_arg_array(t_data *data)
{
	if (data->argv[data->cmd][0] == '\0' || data->argv[data->cmd][0] == ' ')
	{
		print_cmd_nfound(data);
		exit_free_close(NULL, 127, data);
	}
	data->cmd_arg_array = ft_split(data->argv[data->cmd], ' ');
	if (data->cmd_arg_array == NULL)
		exit_free_close("set_cmd_arg_array, malloc error", 1, data);
}

void	check_path_and_exec(t_data *data)
{
	int	i;

	i = 0;
	set_cmd_arg_array(data);
	if (data->paths_array != NULL && ft_strnstr(data->cmd_arg_array[0],
			"/", ft_nstrlen(data->cmd_arg_array[0])) == NULL)
	{
		while (data->paths_array != NULL && data->paths_array[i] != NULL)
		{
			data->paths_array[i] = ft_fstrjoin(data->paths_array[i],
					data->cmd_arg_array[0]);
			if (data->paths_array[i] == NULL)
				exit_free_close("check_cmd_path, malloc error", 1, data);
			if (access(data->paths_array[i], F_OK) == 0)
				exec_cmd(data->paths_array, data->cmd_arg_array, i, data);
			i++;
		}
		if (print_cmd_nfound(data) == 0)
			exit_free_close(NULL, 1, data);
		ft_putstr_fd(data->cmd_arg_array[0], 2);
		exit_free_close(NULL, 127, data);
	}
	exec_cmd(data->cmd_arg_array, data->cmd_arg_array, 0, data);
}
