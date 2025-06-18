/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd_utils.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ecasalin <ecasalin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/15 10:49:58 by ecasalin          #+#    #+#             */
/*   Updated: 2025/06/18 15:04:33 by ecasalin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <errno.h>
#include "../../libft/src/libft.h"
#include "../minishell.h"
#include "builtins.h"

int	is_path_syntax_valid(char *path)
{
	if (ft_strncmp(path, "./", 2) == SUCCESS
		|| ft_strncmp(path, "../", 3) == SUCCESS
		|| ft_strncmp(path, "..", 3) == SUCCESS
		|| ft_strncmp(path, ".", 2) == SUCCESS
		|| ft_strncmp(path, "/", 1) == SUCCESS)
		return (SUCCESS);
	return (ERROR);
}

static int	is_valid_relative_path(char *path)
{
	int	i;

	i = 0;
	if (path[i] != '.')
		return (ERROR);
	while (path[i])
	{
		if (is_path_syntax_valid(&path[i]) == ERROR)
			return (ERROR);
		i++;
	}
	return (SUCCESS);
}

static char	*parse_relative_path(char *path)
{
	int	i;

	i = 0;
	while (path[i])
	{
		if (ft_strncmp(&path[i], "./", 2) == SUCCESS
			|| ft_strncmp(&path[i], "../", 3) == SUCCESS
			|| ft_strncmp(&path[i], "..", 3) == SUCCESS
			|| ft_strncmp(&path[i], ".", 2) == SUCCESS)
			break ;
		i++;
	}
	if (path[i] == '\0')
		return (path);
	return (&path[i]);
}

static t_bool	is_dir_unlinked(char *path)
{
	struct stat	st_buffer;
	int			temp;

	temp = stat(path, &st_buffer);
	if (temp == 0 && st_buffer.st_nlink == 0)
	{
		errno = ENOENT;
		return (true);
	}
	return (false);
}

int	chdir_unlinked_cwd(char *path, t_shell_vars *vars)
{
	int	return_value;

	return_value = ERROR;
	if (is_valid_relative_path(path) == SUCCESS)
	{
		path = concat_path_and_cwd_backup(vars->cwd_backup, path);
		if (path == NULL)
			return (CRIT_ERROR);
		if (!is_dir_unlinked(parse_relative_path(path))
			&& chdir(parse_relative_path(path)) == SUCCESS)
			return_value = update_wd_vars(vars);
		else
		{
			if (errno == ENOENT && (set_wd_var("PWD", path, vars) == CRIT_ERROR
					|| update_cwd_backup(path, vars) == CRIT_ERROR))
				return_value = CRIT_ERROR;
			else
				return_value = ERROR;
		}
		free(path);
	}
	else if (chdir(path) == SUCCESS)
		return_value = update_wd_vars(vars);
	return (return_value);
}
