/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_command.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ecasalin <ecasalin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/14 11:23:07 by ecasalin          #+#    #+#             */
/*   Updated: 2025/06/17 12:15:03 by ecasalin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../libft/src/libft.h"
#include <unistd.h>
#include <errno.h>
#include <stdio.h>
#include <sys/wait.h>
#include "../general_utils/utils.h"
#include "../error_handling/errors.h"
#include "../cleaning_utils/cleaning.h"
#include "../parsing/parsing.h"
#include "../builtins/builtins.h"
#include "exec.h"
#include "../minishell.h"
#include "../expansions/expansions.h"

char	**create_cmd_array(t_dlst *args, t_error *error)
{
	char	**cmd_array;
	int		lst_len;
	int		i;

	i = 0;
	lst_len = ft_dlstsize(args);
	cmd_array = ft_calloc(lst_len + 1, sizeof(char *));
	if (cmd_array == NULL)
	{
		perror("minishell: execution: critical error");
		return (set_err_return_null(error, critical));
	}
	while (args != NULL)
	{
		cmd_array[i] = ft_strdup(get_toklist_str(args));
		if (cmd_array[i] == NULL)
		{
			free_array(cmd_array);
			perror("minishell: execution: critical error");
			return (set_err_return_null(error, critical));
		}
		i++;
		args = args->next;
	}
	return (cmd_array);
}

int	create_exec_setup(t_bin_tree *curr_node, t_shell_vars *vars, t_error *error)
{
	char	**paths_array;
	pid_t	child_pid;
	int		builtin;

	if (perform_expansions(curr_node, vars, error) == ERROR)
		return (ERROR);
	builtin = is_builtin(curr_node);
	if (builtin)
		return (prepare_builtin_exec(builtin, curr_node, vars, error));
	paths_array = create_paths_array(vars, error);
	if (*error != success)
		return (ERROR);
	if (curr_node->parent && curr_node->parent->operator == pipeline)
		prepare_to_exec(curr_node, paths_array, vars, error);
	child_pid = fork();
	if (child_pid == FAILURE)
		return_perror_set_err(
			"minishell: execution: fork", error, recoverable);
	if (child_pid == CHILD)
		prepare_to_exec(curr_node, paths_array, vars, error);
	free_array(paths_array);
	if (*error == success && child_pid != CHILD)
		return (wait_child());
	return (ERROR);
}

static int	exec_command(char *cmd_name, char **args, t_shell_vars *vars)
{
	if (cmd_name != NULL)
		execve(cmd_name, args, vars->env);
	else
		return (SUCCESS);
	if (errno == ENOEXEC || errno == EACCES)
		return (126);
	if (errno == ENOENT)
		return (127);
	else
		return (1);
}

int	exec_relative_path_cmd(char **paths_array,
			char **cmd_array, t_shell_vars *vars, t_bin_tree *curr_node)
{
	int		i;
	char	*temp_line;
	t_error	error;
	int		temp;

	error = success;
	i = -1;
	while (paths_array[++i] != NULL)
	{
		temp_line = free_strjoin(paths_array[i], cmd_array[0], true, false);
		if (temp_line == NULL)
		{
			safe_free_cmd_path(paths_array, i + 1);
			free_all_exit_err(NULL, cmd_array, curr_node, vars);
		}
		paths_array[i] = temp_line;
		if (access(paths_array[i], F_OK) == SUCCESS)
			return (exec_command(paths_array[i], cmd_array, vars));
	}
	temp = print_joined_cmd_error(
			cmd_array[0], NULL, ": command not found\n", &error);
	free_arrays_tree_and_vars(paths_array, cmd_array, curr_node, vars);
	if (temp == ERROR)
		exit (EXIT_FAILURE);
	exit(127);
}

void	prepare_to_exec(t_bin_tree *curr_node,
			char **paths_array, t_shell_vars *vars, t_error *error)
{
	int		exit_value;
	char	**cmd_array;

	set_io_fds(curr_node, error);
	if (*error)
		free_all_exit_err(paths_array, NULL, curr_node, vars);
	cmd_array = create_cmd_array(curr_node->content->tokens_list, error);
	if (*error)
		free_all_exit_err(paths_array, NULL, curr_node, vars);
	if (paths_array != NULL && paths_array[0] != NULL
		&& cmd_array[0] != NULL
		&& ft_strnstr(cmd_array[0], "/", ft_strlen(cmd_array[0])) == NULL)
		exit_value = exec_relative_path_cmd(paths_array,
				cmd_array, vars, curr_node);
	else
		exit_value = exec_command(cmd_array[0], cmd_array, vars);
	exit_value = print_exec_error(cmd_array[0], exit_value, error);
	free_arrays_tree_and_vars(paths_array, cmd_array, curr_node, vars);
	exit(exit_value);
}
