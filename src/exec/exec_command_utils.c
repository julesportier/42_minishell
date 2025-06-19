/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_command_utils.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ecasalin <ecasalin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/19 07:30:51 by ecasalin          #+#    #+#             */
/*   Updated: 2025/06/19 07:45:05 by ecasalin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"
#include "../../libft/src/libft.h"
#include "../parsing/parsing.h"
#include "exec.h"

int	is_invalid_command_name(char *cmd_name)
{
	if (!ft_strncmp(cmd_name, ".", 2)
		|| !ft_strncmp(cmd_name, "..", 3))
		return (1);
	return (0);
}

void	exit_command_not_found(char **cmd_array, char **paths_array,
			t_bin_tree *curr_node, t_shell_vars *vars)
{
	int 	temp;
	t_error	error;
	
	temp = print_joined_cmd_error(
			cmd_array[0], NULL, ": command not found\n", &error);
	free_arrays_tree_and_vars(paths_array, cmd_array, curr_node, vars);
	if (temp == ERROR)
		exit (EXIT_FAILURE);
	exit(127);
}