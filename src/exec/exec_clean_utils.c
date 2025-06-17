/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_clean_utils.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ecasalin <ecasalin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/24 09:30:22 by erik              #+#    #+#             */
/*   Updated: 2025/06/17 11:20:21 by ecasalin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include "../error_handling/errors.h"
#include "../general_utils/utils.h"
#include "../cleaning_utils/cleaning.h"
#include "exec.h"
#include "../minishell.h"
#include <sys/wait.h>
#include <readline/history.h>
#include <readline/readline.h>

int	close_pipe(int *pipe)
{
	int	temp1;
	int	temp2;

	temp1 = close(pipe[READ]);
	temp2 = close(pipe[WRITE]);
	if (temp1 == FAILURE || temp2 == FAILURE)
		return (return_perror("minishell: execution: "
				"error closing pipe", ERROR));
	return (SUCCESS);
}

int	close_pipe_perror(char *err_msg, int return_value, int *pipe)
{
	if (err_msg)
		perror(err_msg);
	close_pipe(pipe);
	return (return_value);
}

void	free_arrays_tree_and_vars(char **paths_array,
		char **cmd_array, t_bin_tree *curr_node, t_shell_vars *vars)
{
	free_array(paths_array);
	free_array(cmd_array);
	free_tree_and_vars(tree_root(curr_node), vars);
}

void	free_all_exit_err(char **paths_array,
		char **cmd_array, t_bin_tree *curr_node, t_shell_vars *vars)
{
	free_array(paths_array);
	free_array(cmd_array);
	free_tree_and_vars(tree_root(curr_node), vars);
	rl_clear_history();
	exit(EXIT_FAILURE);
}

void	safe_free_cmd_path(char **paths_array, int index)
{
	perror("minishell: execution: critical error");
	if (index != 0)
	{
		paths_array[index - 1] = NULL;
		free_array_content(paths_array);
	}
	free_array_content(&paths_array[index]);
	free(paths_array);
}
