/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   path_parsing.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ecasalin <ecasalin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/06 11:28:04 by ecasalin          #+#    #+#             */
/*   Updated: 2025/06/17 10:00:15 by ecasalin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../libft/src/libft.h"
#include <stdlib.h>
#include <unistd.h>
#include "../general_utils/utils.h"
#include "../error_handling/errors.h"
#include "../cleaning_utils/cleaning.h"
#include "exec.h"
#include "../minishell.h"

char	**create_paths_array(t_shell_vars *vars, t_error *error)
{
	char	**paths_array;
	char	*path_var;

	*error = success;
	if (vars->env == NULL)
		return (NULL);
	path_var = get_env_var_value("PATH", vars->env);
	if (path_var == NULL)
		return (NULL);
	paths_array = ft_split(path_var, ':');
	if (paths_array == NULL)
		return (return_perror_set_err_null(
				"minishell: execution: critical error", error, critical));
	if (add_slash_to_paths(paths_array) == CRIT_ERROR)
	{
		free_array(paths_array);
		return (return_perror_set_err_null(
				"minishell: execution: critical error", error, critical));
	}
	return (paths_array);
}
