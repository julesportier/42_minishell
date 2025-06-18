/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cleaning_utils_1.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ecasalin <ecasalin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/25 17:57:36 by ecasalin          #+#    #+#             */
/*   Updated: 2025/06/18 13:13:07 by ecasalin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include <unistd.h>
#include "../parsing/parsing.h"
#include "../minishell.h"
#include "../exec/exec.h"
#include "../error_handling/errors.h"
#include <readline/readline.h>

void	free_array_content(char **array)
{
	int	i;

	i = 0;
	if (array == NULL)
		return ;
	while (array[i])
	{
		free(array[i]);
		i++;
	}
}

void	free_array(char **array)
{
	free_array_content(array);
	free(array);
}

void	free_tree_and_vars(t_bin_tree *tree_root, t_shell_vars *vars)
{
	free_tree(&tree_root);
	free_array(vars->env);
	free(vars->cwd_backup);
	free(vars->prompt);
	rl_clear_history();
}

int	return_close_pipe_perror(char *err_msg, int pip[2],
	t_error *error, t_error err_val)
{
	close(pip[READ]);
	close(pip[WRITE]);
	return (perror_set_err(err_msg, error, err_val));
}
