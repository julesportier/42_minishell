/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd_cdpath_utils.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ecasalin <ecasalin@42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/15 10:50:49 by ecasalin          #+#    #+#             */
/*   Updated: 2025/06/09 08:43:13 by ecasalin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <unistd.h>
#include <errno.h>
#include <stdio.h>
#include "../minishell.h"
#include "builtins.h"
#include "../../libft/src/libft.h"
#include "../general_utils/utils.h"
#include "../cleaning_utils/cleaning.h"

static char	**create_cdpath_array(char *cdpath)
{
	char	**cdpath_array;

	cdpath_array = ft_split(cdpath, ':');
	if (cdpath_array == NULL)
		return (NULL);
	if (add_slash_to_paths(cdpath_array) == CRIT_ERROR)
	{
		free_array (cdpath_array);
		return (NULL);
	}
	return (cdpath_array);
}

/* cdpath can't be NULL and cdpath[0] will never be '\0' */
static int	iter_cdpath(char *path, char *cdpath, t_shell_vars *vars)
{
	char	**cdpath_array;
	int		i;
	char	*temp_path;
	int		return_value;

	cdpath_array = create_cdpath_array(cdpath);
	if (cdpath_array == NULL)
		return (CRIT_ERROR);
	return_value = ERROR;
	i = -1;
	while (cdpath_array[++i])
	{
		temp_path = ft_strjoin(cdpath_array[i], path);
		if (temp_path == NULL)
			return_value = CRIT_ERROR;
		if (return_value != CRIT_ERROR)
			return_value = chdir_update_wd(temp_path, vars);
		if (return_value == SUCCESS)
			printf("%s\n", temp_path);
		free(temp_path);
		if (return_value != ERROR)
			break ;
	}
	free_array(cdpath_array);
	return (return_value);
}

static int	cd_in_curr_dir(char *path, t_shell_vars *vars)
{
	int		return_value;
	char	*temp_path;

	temp_path = ft_strjoin("./", path);
	if (temp_path == NULL)
		return (CRIT_ERROR);
	return_value = chdir_update_wd(temp_path, vars);
	free(temp_path);
	return (return_value);
}

int	test_possible_paths(char *path, t_shell_vars *vars)
{
	char	*cdpath;
	int		return_value;

	return_value = ERROR;
	cdpath = get_env_var_value("CDPATH", vars->env);
	if (cdpath == NULL || cdpath[0] == ':' || cdpath[0] == '\0')
		return_value = cd_in_curr_dir(path, vars);
	if (return_value == ERROR && cdpath != NULL)
		return_value = iter_cdpath(path, cdpath, vars);
	if (return_value == ERROR && cdpath != NULL)
		return_value = cd_in_curr_dir(path, vars);
	return (return_value);
}
