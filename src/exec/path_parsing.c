/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   path_parsing.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ecasalin <ecasalin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/06 11:28:04 by ecasalin          #+#    #+#             */
/*   Updated: 2025/05/09 09:26:12 by ecasalin         ###   ########.fr       */
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

static char	*extract_path_var(char **envp, t_shell_vars *vars)
{
	int		i;
	char	*path_var;

	i = 0;
	while (envp[i] != NULL)
	{
		if (ft_strncmp(envp[i], "PATH=", 5) == 0)
		{
			if (envp[i][5] == '\0')
				return (NULL);
			path_var = ft_strdup(envp[i]);
			// if (path_var == NULL)
			// 	exit_free_close("extrac_path_var, malloc error", 1, data);
			// else
				return (path_var);
		}
		i++;
	}
	return (NULL);
}

static char	**parse_path_to_array(char *path_var)
{
	char	**paths_array;

	paths_array = ft_split(path_var + 5, ':');
	free(path_var);
	return (paths_array);
}

char	**create_paths_array(t_shell_vars *vars)
{
	char	**paths_array;
	char	*path_var;

	if (vars->env == NULL)
		return (NULL);
	path_var = extract_path_var(vars->env, vars);
	// if (path_var == NULL)
	// 	return (NULL);
	paths_array = parse_path_to_array(path_var);
	// if (paths_array == NULL)
	// 	exit_free_close("parse_path_to_array, malloc error", 1, data);
	// if (add_slash_to_paths(paths_array) == 0)     !!!!!!!!!!!!!!!! SI ADD_SLASHS_TO_ARRAY FAIL IL FAUT FREE LE TABLEAU DES PATHS !!!!!!!!!!!!
	// 	exit_free_close("add_slash_to_paths, malloc error", 1, data);
	return (paths_array);
}
