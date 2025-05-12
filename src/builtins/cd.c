/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ecasalin <ecasalin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/24 07:21:49 by ecasalin          #+#    #+#             */
/*   Updated: 2025/05/12 16:02:44 by ecasalin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <unistd.h>
#include <stdio.h>
#include <errno.h>
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
	err_msg = free_strjoin(err_msg, ": No such file or directory\n", true, false);
	if (err_msg == NULL)
		return (CRIT_ERROR);
	ft_putstr_fd(err_msg, 2);
	free(err_msg);
	return (ERROR);
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

int	set_wd_var(char *var_key, char *var_value, t_shell_vars *vars)
{
	char	**temp_env;
	char	*temp_env_var;
	int		return_value;
	
	var_value = join_pwds_key_value(var_key, var_value);
	if (var_value == NULL)
		return (CRIT_ERROR);
	return_value = SUCCESS;
	temp_env_var = get_env_var(var_key, vars->env);
	if (temp_env_var == NULL)
	{
		temp_env = add_var_to_env(var_value, vars->env);
		if (temp_env == NULL)
		{
			free(var_value);
			return (CRIT_ERROR);
		}
		vars->env = temp_env;
	}
	else
		return_value = update_var(temp_env_var, var_value, vars->env);
	free(var_value);
	return (return_value);
}

int	chdir_set_wd_var(char *path, t_shell_vars *vars)
{
	int		return_value;
	char	*temp_wd;
	
	temp_wd = ft_strdup(get_env_var_value("PWD", vars->env));
	if (errno == ENOMEM)
		return (CRIT_ERROR);
	return_value = ERROR;
	if (chdir(path) == SUCCESS)
	{
		return_value = set_wd_var("OLDPWD", temp_wd, vars);
		if (return_value == CRIT_ERROR)
			return (CRIT_ERROR);
		temp_wd = getcwd(NULL, 0);
		if (temp_wd == NULL)
			return (CRIT_ERROR);
		return_value = set_wd_var("PWD", temp_wd, vars);
	}
	else
		free(temp_wd);
	return (return_value);
}

int	cd_to_home(t_shell_vars *vars)
{
	char	*path;
	int		return_value;

	path = get_env_var_value("HOME", vars->env);
	if (path == NULL)
		return (return_error("minishell: cd: HOME not set\n", ERROR));
	return_value = chdir_set_wd_var(path, vars);
	if (return_value == CRIT_ERROR)
		return (return_perror("minishell: cd critical error", CRIT_ERROR));
	if (return_value == ERROR)
		return (return_print_no_path_err(path));
	return (SUCCESS);
}

int	cd_to_oldpwd(t_shell_vars *vars)
{
	char	*path;
	char	*temp_path;
	int		return_value;
	
	path = get_env_var_value("OLDPWD", vars->env);
	if (path == NULL)
		return (return_error("minishell: cd: OLDPWD not set\n", ERROR));
	temp_path = ft_strdup(path);
	if (temp_path == NULL)
		return (return_perror("minishell: cd critical error", CRIT_ERROR));
	return_value = chdir_set_wd_var(path, vars);
	if (return_value == CRIT_ERROR)
		return (return_perror("minishell: cd critical error", CRIT_ERROR));
	if (return_value == ERROR)
		return (return_print_no_path_err(path));
	printf("%s\n", temp_path);
	free(temp_path);
	return (SUCCESS);
}

char	**create_cdpath_array(char *cdpath)
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
int	iter_cdpath(char *path, char *cdpath, t_shell_vars *vars)
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
		if (return_value != CRIT_ERROR && chdir_set_wd_var(temp_path, vars) == SUCCESS)
			return_value = SUCCESS;
		if (return_value == SUCCESS)
			printf("%s\n", temp_path);
		free(temp_path);
		if (return_value != ERROR)
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
		return_value = chdir_set_wd_var(temp_path, vars);
		free(temp_path);
	}
	if (return_value == ERROR && cdpath != NULL)
		return_value = iter_cdpath(path, cdpath, vars);
	return (return_value);
}

static int	is_path_syntax_valid(char *path)
{
	if (ft_strncmp(path, "./", 2) == SUCCESS ||
		ft_strncmp(path, "../", 3) == SUCCESS ||
		ft_strncmp(path, "..", 3) == SUCCESS ||
		ft_strncmp(path, "/", 1) == SUCCESS)
		return (SUCCESS);
	return (ERROR);
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
		return (cd_to_home(vars));
	else if (ft_strncmp(args[0], "-", 2) == SUCCESS)
		return (cd_to_oldpwd(vars));
	if (is_path_syntax_valid(args[0]) == SUCCESS)
		return_value = chdir_set_wd_var(args[0], vars);
	else
		return_value = test_possible_paths(args[0], vars);
	if (return_value == CRIT_ERROR)
		return (return_perror("minishell: cd critical error:", CRIT_ERROR));
	else if (return_value == ERROR)
		return (return_print_no_path_err(args[0]));
	return (SUCCESS);
}

#include "../shell_init/init.h"
int	main(int ac, char *av[], char *envp[])
{
	t_shell_vars	vars;
	t_error	error;
	// int		i = 0;
	
	// while (envp[i])
	// {
	// 	printf("%s   %d\n", envp[i], i);
	// 	i++;
	// }
	vars.env = init_env_array(envp, &error);
	printf("Initial PWD var value : %s\n", get_env_var_value("PWD", vars.env));
	printf("Initial OLDPWD var value : %s\n------------------------------------------------------------------\n", get_env_var_value("OLDPWD", vars.env));
	ms_cd(&av[2], &vars);
	printf("Updated PWD var value : %s\n", get_env_var_value("PWD", vars.env));
	printf("Updated OLDPWD var value : %s\nGG !\n", get_env_var_value("OLDPWD", vars.env));
	free_array(vars.env);
}