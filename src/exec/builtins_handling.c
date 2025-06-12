/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins_handling.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ecasalin <ecasalin@42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/26 13:13:02 by ecasalin          #+#    #+#             */
/*   Updated: 2025/06/12 08:46:20 by ecasalin         ###   ########.fr       */
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

	if (curr_node->content->tokens_list == NULL)
		return (not_builtin);
	cmd_name = get_toklist_str(curr_node->content->tokens_list);
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

int	exec_builtin(t_builtin builtin, char **cmd_array,
		t_shell_vars *vars, t_error *error)
{
	int	return_value;

	return_value = SUCCESS;
	if (builtin == echo)
		return_value = ms_echo(&cmd_array[1]);
	else if (builtin == cd)
		return_value = ms_cd(&cmd_array[1], vars);
	else if (builtin == pwd)
		return_value = ms_pwd();
	else if (builtin == env)
		return_value = ms_env(vars->env);
	else if (builtin == export)
		return_value = ms_export(&cmd_array[1], vars);
	else if (builtin == unset)
		return_value = ms_unset(&cmd_array[1], vars);
	if (return_value == CRIT_ERROR)
		return (set_err_return_err(error, critical));
	if (builtin == ext)
		return_value = ms_exit(cmd_array, vars, error);
	return (return_value);
}

static int	restore_fds_free_array(int std_shell_fds[2],
			char **array, t_error *error)
{
	if (array != NULL)
		free_array(array);
	if (*error == recoverable)
		restore_shell_fds(std_shell_fds);
	else
	{
		close(std_shell_fds[0]);
		close(std_shell_fds[1]);
	}
	return (ERROR);
}

int	prepare_builtin_exec(t_builtin builtin, t_bin_tree *curr_node,
		t_shell_vars *vars, t_error *error)
{
	int		std_shell_fds[2];
	char	**cmd_array;
	int		return_value;

	cmd_array = create_cmd_array(curr_node->content->tokens_list, error);
	if (*error)
		return (ERROR);
	if (save_shell_fds(std_shell_fds) == ERROR)
		return (free_array_set_err(error, recoverable, cmd_array));
	set_io_fds(curr_node, error);
	if (*error)
		return (restore_fds_free_array(std_shell_fds, cmd_array, error));
	return_value = exec_builtin(builtin, cmd_array, vars, error);
	free_array(cmd_array);
	if (*error == recoverable || *error == critical)
		return (restore_fds_free_array(std_shell_fds, NULL, error));
	if (restore_shell_fds(std_shell_fds) == ERROR)
		return (set_err_return_err(error, recoverable));
	if (builtin == ext && *error != invalid_exit_args)
	{
		free_tree_and_vars(tree_root(curr_node), vars);
		exit(return_value);
	}
	return (return_value);
}
