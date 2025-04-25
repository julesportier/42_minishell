/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   path_parsing.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kura <kura@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/06 11:28:04 by ecasalin          #+#    #+#             */
/*   Updated: 2025/04/25 18:11:59 by kura             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "exec.h"

static char	*extract_path_var(char **envp, t_var_data *vars)
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

static int	add_slash_to_paths(char **paths_array)
{
	int		i;
	char	*slashed_line;

	i = 0;
	while (paths_array[i] != NULL)
	{
		slashed_line = ft_fstrjoin(paths_array[i], "/");
		if (slashed_line == NULL)
		{
			free_array(paths_array);
			return (ERROR);
		}
		paths_array[i] = slashed_line;
		i++;
	}
	return (SUCCESS);
}

char	**create_paths_array(t_var_data *vars)
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
	// if (add_slash_to_paths(paths_array) == 0)
	// 	exit_free_close("add_slash_to_paths, malloc error", 1, data);
	return (paths_array);
}
