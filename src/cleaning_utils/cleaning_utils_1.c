/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cleaning_utils_1.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ecasalin <ecasalin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/25 17:57:36 by ecasalin          #+#    #+#             */
/*   Updated: 2025/05/26 13:42:04 by ecasalin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
// #include <stdio.h>
#include <unistd.h>
#include "../parsing/parsing.h"
// #include "../minishell.h"

void	free_array(char **array)
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
	free(array);
}

void	free_tree_and_vars(t_bin_tree *tree_root, t_shell_vars *vars)
{
	free_tree(&tree_root);
	free_array(vars->env);
	free(vars->cwd_backup);
	free(vars->prompt);
}
