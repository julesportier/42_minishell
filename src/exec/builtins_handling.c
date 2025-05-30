/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins_handling.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ecasalin <ecasalin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/26 13:13:02 by ecasalin          #+#    #+#             */
/*   Updated: 2025/05/30 16:30:01 by ecasalin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <unistd.h>
#include <stdio.h>
#include "../minishell.h"
#include "../builtins/builtins.h"
#include "../parsing/parsing.h"
#include "../cleaning_utils/cleaning.h"
#include "../error_handling/errors.h"
#include "exec.h"

int	is_builtin(t_bin_tree *curr_node)
{
	char	*cmd_name;

	cmd_name = get_toklist_str(curr_node->content->tokens_list);
	if (cmd_name == NULL)
		return (not_builtin);
	if (ft_strncmp(cmd_name, "echo", 5) == SUCCESS)
		return (echo);
	if (ft_strncmp(cmd_name, "cd", 3) == SUCCESS)
		return (cd);
	if (ft_strncmp(cmd_name, "pwd", 4) == SUCCESS)
		return (pwd);
	if (ft_strncmp(cmd_name, "env", 4) == SUCCESS)
		return (env);
	if (ft_strncmp(cmd_name, "export", 7) == SUCCESS)
		return (export);
	if (ft_strncmp(cmd_name, "unset", 6) == SUCCESS)
		return (unset);
	if (ft_strncmp(cmd_name, "exit", 5) == SUCCESS)
		return (ext);
	return (not_builtin);
}

int	exec_builtin(t_builtin builtin, char **cmd_array, t_shell_vars *vars, t_error *error)
{
	int				return_value;
	t_exit_error	exit_error;
	
	exit_error = no_error;
	if (builtin == echo)
		return_value = ms_echo(&cmd_array[1]);
	if (builtin == cd)
		return_value = ms_cd(&cmd_array[1], vars);
	if (builtin == pwd)
		return_value = ms_pwd();
	if (builtin == env)
		return_value = ms_env(vars->env);
	if (builtin == export)
		return_value = ms_export(&cmd_array[1], vars);
	if (builtin == unset)
		return_value = ms_unset(&cmd_array[1], vars);
	if (builtin == ext)
	{
		return_value = ms_exit(cmd_array, vars, &exit_error);
		if (exit_error == alloc_error)
			return (set_err_return_err(error, critical));
		if (exit_error == not_a_digit || exit_error == too_many_args)
			return (set_err_return_err(error, recoverable));
	}
	if (return_value == CRIT_ERROR)
		return (set_err_return_err(error, critical));
	return (return_value);
}

static int	free_array_return_perror(int return_value, char **array)
{
	perror("minishell: execution: redirection error");
	free_array(array);
	return (return_value);
}


int	prepare_builtin_exec(t_builtin builtin, t_bin_tree *curr_node, t_shell_vars *vars, t_error *error)
{
	int		std_shell_fds[2];
	char	**cmd_array;
	int		return_value;
	
	cmd_array = create_cmd_array(curr_node->content->tokens_list, error);
	if (*error)
		return (ERROR);
	if (save_shell_fds(std_shell_fds) != SUCCESS)
		return (free_array_set_err(error, recoverable, cmd_array));
	return_value = set_io_fds(curr_node, error);
	if (return_value != SUCCESS)
		return (free_array_return_perror(return_value, cmd_array));
	return_value = exec_builtin(builtin, cmd_array, vars, error);
	free_array(cmd_array);
	if (*error == critical)
		return (CRIT_ERROR);
	if (reset_shell_fds(std_shell_fds) == ERROR)
		return (ERROR);
	if (builtin == ext && *error == success)
	{
		free_tree_and_vars(tree_root(curr_node), vars);
		exit(return_value);
	}
	return (return_value);
}
