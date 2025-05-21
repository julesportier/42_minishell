/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ecasalin <ecasalin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/24 07:21:49 by ecasalin          #+#    #+#             */
/*   Updated: 2025/05/21 13:46:41 by ecasalin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <unistd.h>
#include <fcntl.h>
#include <stdio.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <string.h>
#include "../../libft/src/libft.h"
#include "../minishell.h"
#include "../general_utils/utils.h"
#include "../error_handling/errors.h"
#include "../cleaning_utils/cleaning.h"
#include "builtins.h"

static int	return_print_no_path_err(char *path)
{
	char	*err_msg;

	err_msg = ft_strdup_s("minishell: cd: ");
	if (err_msg == NULL)
		return (CRIT_ERROR);
	err_msg = free_strjoin(err_msg, path, true, false);
	if (err_msg == NULL)
		return (CRIT_ERROR);
	err_msg = free_strjoin(err_msg, ": ", true, false);
	if (err_msg == NULL)
		return (CRIT_ERROR);
	err_msg = free_strjoin(err_msg, strerror(errno), true, false);
	if (err_msg == NULL)
		return (CRIT_ERROR);
	err_msg = free_strjoin(err_msg, "\n", true, false);
	if (err_msg == NULL)
		return (CRIT_ERROR);
	ft_putstr_fd(err_msg, 2);
	free(err_msg);
	return (ERROR);
}

static int	cd_to_home(t_shell_vars *vars)
{
	char	*path;
	int		return_value;

	path = get_env_var_value("HOME", vars->env);
	if (path == NULL)
		return (return_error("minishell: cd: HOME not set\n", ERROR));
	return_value = chdir_update_wd_vars(path, vars);
	if (return_value == CRIT_ERROR)
		return (return_perror("minishell: cd critical error", CRIT_ERROR));
	if (return_value == ERROR)
		return (return_print_no_path_err(path));
	return (SUCCESS);
}

static int	cd_to_oldpwd(t_shell_vars *vars)
{
	char	*path;
	char	*temp_path;
	int		return_value;

	path = get_env_var_value("OLDPWD", vars->env);
	if (path == NULL)
		return (return_error("minishell: cd: OLDPWD not set\n", ERROR));
	temp_path = ft_strdup_s(path);
	if (temp_path == NULL)
		return (return_perror("minishell: cd critical error", CRIT_ERROR));
	return_value = chdir_update_wd_vars(path, vars);
	if (return_value == SUCCESS)
		printf("%s\n", temp_path);
	free(temp_path);
	if (return_value == CRIT_ERROR)
		return (return_perror("minishell: cd critical error", CRIT_ERROR));
	if (return_value == ERROR)
		return (return_print_no_path_err(path));
	return (SUCCESS);
}

int	chdir_update_wd_vars(char *path, t_shell_vars *vars)
{
	struct stat	st_buffer;
	int			temp;
	
	if (access("./", X_OK) == SUCCESS)
	{
		temp = stat("./", &st_buffer);
		if (temp < 0 || st_buffer.st_nlink == 0)
			return (chdir_unlinked_cwd(path, vars));
	}
	if (chdir(path) == SUCCESS)
		return (update_wd_vars(vars));
	else
		return (ERROR);
}

int	ms_cd(char **args, t_shell_vars *vars)
{
	int		args_number;
	int		return_value;

	args_number = count_array_len(args);
	if (args_number > 1)
		return (return_error("minishell: cd: to many arguments", ERROR));
	else if (args_number == 0)
		return (cd_to_home(vars));
	else if (ft_strncmp(args[0], "-", 2) == SUCCESS)
		return (cd_to_oldpwd(vars));
	if (is_path_syntax_valid(args[0]) == SUCCESS)
		return_value = chdir_update_wd_vars(args[0], vars);
	else
		return_value = test_possible_paths(args[0], vars);
	if (return_value == CRIT_ERROR)
		return (return_perror("minishell: cd critical error", CRIT_ERROR));
	if (return_value == ERROR)
		return (return_print_no_path_err(args[0]));
	return (SUCCESS);
}

// #include "../shell_init/init.h"
// int	main(int ac, char *av[], char *envp[])
// {
// 	t_shell_vars	vars;
// 	t_error	error;
// 	int	ret;
// 	// int		i = 0;

// 	// while (envp[i])
// 	// {
// 	// 	printf("%s   %d\n", envp[i], i);
// 	// 	i++;
// 	// }
// 	vars.env = init_env_array(envp, &error);
// 	vars.cwd_backup = init_cwd_backup(vars.env);
// 	if (vars.cwd_backup == NULL)
// 		return (printf("Backup malloc error\n"));
// 	printf("Initial BACKUP var value : %s\n", vars.cwd_backup);
// 	printf("Initial PWD var value : %s\n", get_env_var_value("PWD", vars.env));
// 	printf("Initial OLDPWD var value : %s\n------------------------------------------------------------------\n", get_env_var_value("OLDPWD", vars.env));
// 	ret = ms_cd(&av[2], &vars);
// 	printf("Updated BACKUP var value : %s\n", vars.cwd_backup);
// 	printf("Updated PWD var value : %s\n", get_env_var_value("PWD", vars.env));
// 	printf("Updated OLDPWD var value : %s\nGG !\n", get_env_var_value("OLDPWD", vars.env));
// 	free_array(vars.env);
// 	free(vars.cwd_backup);
// 	return (ret);
// 	// int ret;

// 	// ret = is_valid_relative_path(av[1]);
// 	// if (ret == SUCCESS)
// 	// 	printf("SUCCESS : %s\n", av[1]);
// 	// else
// 	// 	printf("ERROR : %s\n", av[1]);
// }
