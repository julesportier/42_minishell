/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd_cwd_vars_utils.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ecasalin <ecasalin@42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/15 10:53:03 by ecasalin          #+#    #+#             */
/*   Updated: 2025/06/09 09:07:48 by ecasalin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <unistd.h>
#include <errno.h>
#include <stdlib.h>
#include "../../libft/src/libft.h"
#include "../minishell.h"
#include "builtins.h"
#include "../general_utils/utils.h"
#include "../error_handling/errors.h"

static char	*join_pwds_key_value(char *pwd_key, char *pwd_value)
{
	char	*pwd;

	pwd = ft_strjoin(pwd_key, "=");
	if (pwd == NULL)
		return (NULL);
	pwd = free_strjoin(pwd, pwd_value, true, false);
	return (pwd);
}

int	set_wd_var(char *var_key, char *var_value, t_shell_vars *vars)
{
	char	*var_key_value;
	char	**temp_env;
	char	*temp_env_var;
	int		return_value;

	var_key_value = join_pwds_key_value(var_key, var_value);
	if (var_key_value == NULL)
		return (CRIT_ERROR);
	return_value = SUCCESS;
	temp_env_var = get_env_var(var_key, vars->env);
	if (temp_env_var == NULL)
	{
		temp_env = add_var_to_env(var_key_value, vars->env);
		if (temp_env == NULL)
		{
			free(var_key_value);
			return (CRIT_ERROR);
		}
		vars->env = temp_env;
	}
	else
		return_value = update_var(temp_env_var, var_key_value, vars->env);
	free(var_key_value);
	return (return_value);
}

int	update_cwd_backup(char *new_path, t_shell_vars *vars)
{
	free(vars->cwd_backup);
	vars->cwd_backup = ft_strdup_s(new_path);
	if (vars->cwd_backup == NULL)
		return (CRIT_ERROR);
	return (SUCCESS);
}

char	*concat_path_and_cwd_backup(char *cwd_backup, char *path)
{
	char	*formatted_path;

	formatted_path = ft_strdup_s(cwd_backup);
	if (formatted_path == NULL)
		return (NULL);
	if (ft_strncmp(formatted_path, "/", 2) != SUCCESS
		&& get_last_char(formatted_path) != '/')
		formatted_path = free_strjoin(formatted_path, "/", true, false);
	if (formatted_path == NULL)
		return (NULL);
	formatted_path = free_strjoin(formatted_path, path, true, false);
	return (formatted_path);
}

int	update_wd_vars(t_shell_vars *vars)
{
	char	*temp_wd;
	int		return_value;

	return_value = SUCCESS;
	temp_wd = getcwd(NULL, 0);
	if (temp_wd == NULL && errno == ENOMEM)
		return (CRIT_ERROR);
	if (temp_wd == NULL)
		return (ERROR);
	if (set_wd_var("OLDPWD", get_env_var_value("PWD", vars->env),
			vars) == CRIT_ERROR || set_wd_var("PWD", temp_wd,
			vars) == CRIT_ERROR || update_cwd_backup(temp_wd,
			vars) == CRIT_ERROR)
		return_value = CRIT_ERROR;
	free(temp_wd);
	return (return_value);
}
