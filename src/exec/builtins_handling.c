/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins_handling.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ecasalin <ecasalin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/26 13:13:02 by ecasalin          #+#    #+#             */
/*   Updated: 2025/05/26 13:31:06 by ecasalin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <unistd.h>
#include <stdio.h>
#include "../minishell.h"
#include "../builtins/builtins.h"
#include "../parsing/parsing.h"
#include "../cleaning_utils/cleaning.h"
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

int	exec_builtin(t_builtin builtin, char **cmd_array, t_shell_vars *vars, t_exit_error *exit_error)
{
	if (builtin == echo)
		return (ms_echo(&cmd_array[1]));
	if (builtin == cd)
		return (ms_cd(&cmd_array[1], vars));
	if (builtin == pwd)
		return (ms_pwd());
	if (builtin == env)
		return (ms_env(vars->env));
	if (builtin == export)
		return (ms_export(&cmd_array[1], vars));
	if (builtin == unset)
		return (ms_unset(&cmd_array[1], vars));
	if (builtin == ext)
		return (ms_exit(cmd_array, vars, exit_error));
	return (SUCCESS);
}

static int	free_array_return_perror(int return_value, char **array)
{
	perror("minishell: execution: redirection error");
	free_array(array);
	return (return_value);
}

int	prepare_builtin_exec(t_builtin builtin, t_bin_tree *curr_node, t_shell_vars *vars)
{
	int				std_shell_fds[2];
	char			**cmd_array;
	int				return_value;
	t_exit_error	exit_error;
	
	exit_error = no_error;
	cmd_array = create_cmd_array(curr_node->content->tokens_list);
	if (cmd_array == NULL)
		return (CRIT_ERROR);
	if (save_shell_fds(std_shell_fds) != SUCCESS)
		return (free_array_return_perror(ERROR, cmd_array));
	return_value = set_io_fds(curr_node);
	if (return_value != SUCCESS)
		return (free_array_return_perror(return_value, cmd_array));
	return_value = exec_builtin(builtin, cmd_array, vars, &exit_error);
	free_array(cmd_array);
	if (return_value == CRIT_ERROR)
		return (CRIT_ERROR);
	return_value = reset_shell_fds(std_shell_fds);
	if (builtin == ext && exit_error == success)
	{
		free_tree_and_vars(tree_root(curr_node), vars);
		exit(return_value);
	}
	return (return_value);
}
