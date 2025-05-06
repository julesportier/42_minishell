/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ecasalin <ecasalin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/24 07:21:49 by ecasalin          #+#    #+#             */
/*   Updated: 2025/05/06 15:02:29 by ecasalin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <unistd.h>
#include "../../libft/src/libft.h"
#include "../minishell.h"
#include "../general_utils/utils.h"
#include "../error_handling/errors.h"
#include "../cleaning_utils/cleaning.h"
#include "builtins.h"

int	return_print_no_path_err(char *path)
{
	char	*err_msg;

	err_msg = ft_strdup("minishell: cd: ");
	if (err_msg == NULL)
		return (CRIT_ERROR);
	err_msg = free_strjoin(err_msg, path, true, false);
	if (err_msg == NULL)
		return (CRIT_ERROR);
	err_msg = free_strjoin(err_msg, ": No such file or directory", true, false);
	if (err_msg == NULL)
		return (CRIT_ERROR);
	ft_putendl_fd(err_msg, 2);
	free(err_msg);
	return (ERROR);
}

char	*cd_to_home(t_shell_vars *vars)
{
	char	*path;
	int		return_value;

	path = get_env_var_value("HOME", vars->env);
	if (path == NULL)
		return (return_error("minishell: cd: HOME not set\n", ERROR));
	return_value = chdir_set_pwds(path, vars);
	if (return_value == CRIT_ERROR)
		return (return_perror("minishell: cd critical error", CRIT_ERROR));
	if (return_value == ERROR)
		return (return_print_no_path_err(path));
	return (SUCCESS);
}

char	*cd_to_oldpwd(t_shell_vars *vars)
{
	char	*path;
	int		return_value;

	path = get_env_var_value("OLDPWD", vars->env);
	if (path == NULL)
		return (return_error("minishell: cd: OLDPWD not set\n", ERROR));
	return_value = chdir_set_pwds(path, vars);
	if (return_value == CRIT_ERROR)
		return (return_perror("minishell: cd critical error", CRIT_ERROR));
	if (return_value == ERROR)
		return (return_print_no_path_err(path));
	ft_putendl_fd(path, 1);
	return (SUCCESS);
}

/* cdpath_value can't be NULL and cdpath_value[0] will never be '\0' */
int	iter_cdpath(char *path, char *cdpath, t_shell_vars *vars)
{
	char	**cdpath_array;
	int		i;
	char	*temp_path;
	int		return_value;

	cdpath_array = ft_split(cdpath, ':');
	if (cdpath_array == NULL)
		return (CRIT_ERROR);
	return_value = ERROR;
	i = -1;
	while (cdpath_array[++i])
	{
		temp_path = ft_strjoin(cdpath_array[i], path);
		if (temp_path == NULL)
			return_value = CRIT_ERROR;
		if (return_value != CRIT_ERROR && chdir_set_pwds(temp_path, vars) == SUCCESS)
			return_value = SUCCESS;
		if (return_value == SUCCESS)
			ft_putendl_fd(temp_path, 1);
		free(temp_path);
		if (return_value != ERROR);
			break;
	}
	free_array(cdpath_array);
	return (return_value);
}

int	test_possible_paths(char *path, t_shell_vars *vars)
{
	char	*cdpath;
	char	*temp_path;
	int		return_value;
	
	return_value = ERROR;
	temp_path = NULL;
	cdpath = get_env_var_value("CDPATH", vars->env);
	if (cdpath == NULL || cdpath[0] == ':' || cdpath[0] == '\0')
	{
		temp_path = ft_strjoin("./", path);
		if (temp_path == NULL)
			return (CRIT_ERROR);
		return_value = chdir_set_pwds(temp_path, vars);
		free(temp_path);
	}
	if (return_value == ERROR && cdpath != NULL)
		return_value = iter_cdpath(path, cdpath, vars);
	return (return_value);
}

static char	*join_pwds_key_value(char *pwd_key, char *pwd_value)
{
	char	*pwd;

	pwd = ft_strjoin(pwd_key, "=");
	if (pwd == NULL)
		return (NULL);
	pwd = free_strjoin(pwd, pwd_value, true, true);
	return (pwd);
}

int	set_pwds(char *pwd_value, char *pwd_var_key, t_shell_vars *vars)
{
	char	**temp_env;
	char	*temp_env_var;
	int		return_value;
	
	pwd_value = join_pwds_key_value(pwd_var_key, pwd_value);
	if (pwd_value == NULL)
		return (CRIT_ERROR);
	return_value = SUCCESS;
	temp_env_var = get_env_var(pwd_var_key, vars->env);
	if (temp_env_var == NULL)
	{
		temp_env = add_var_to_env(pwd_value, vars->env);
		if (temp_env == NULL)
		{
			free(pwd_value);
			return (CRIT_ERROR);
		}
		vars->env = temp_env;
	}
	else
		return_value = update_var(temp_env_var, pwd_value, vars->env);
	free(pwd_value);
	return (return_value);
}

int	chdir_set_pwds(char *path, t_shell_vars *vars)
{
	int		return_value;
	char	*temp_pwd;
	
	temp_pwd = getcwd(NULL, 0);
	if (temp_pwd == NULL)
		return (CRIT_ERROR);
	return_value = ERROR;
	if (chdir(path) == SUCCESS)
	{
		return_value = set_pwds(temp_pwd, "OLDPWD", vars);
		free(temp_pwd);
		if (return_value == CRIT_ERROR)
			return (CRIT_ERROR);
		temp_pwd = getcwd(NULL, 0);
		if (temp_pwd == NULL)
			return (CRIT_ERROR);
		return_value = set_pwds(temp_pwd, "PWD", vars);
		free(temp_pwd);
	}
	return (return_value);
}

int	ms_cd(char **args, t_shell_vars *vars)
{
	int		args_number;
	char	*path;
	int		return_value;
	
	args_number = count_array_len(args);
	if (args_number > 1)
		return (return_error("minishell: cd: to many arguments", ERROR));
	else if (args_number == 0)
		return (cd_to_home(vars->env));
	else if (ft_strncmp(args[0], "-", 2) == SUCCESS)
		return (cd_to_oldpwd(vars->env));
	if (ft_strncmp(args[0], "./", 2) == SUCCESS || ft_strncmp(args[0], "../", 3) == SUCCESS || ft_strncmp(args[0], "/", 1) == SUCCESS)
		return_value = chdir_set_pwds(args[0], vars);
	else
		return_value = test_possible_paths(args[0], vars->env);
	if (return_value == CRIT_ERROR)
		return (return_perror("minishell: cd critical error:", CRIT_ERROR));
	if (return_value == ERROR)
		return (return_print_no_path_err(args[0]));
	if (return_value == SUCCESS)
		return (SUCCESS);
}
