/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd_exec.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ecasalin <ecasalin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: Invalid date        by                   #+#    #+#             */
/*   Updated: 2025/05/26 13:30:04 by ecasalin         ###   ########.fr       */
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

// int	prepare_to_exec(t_node *curr_node, char **paths_array, t_shell_vars *vars)
// {
// 	// expand_args(curr_node->cmd.cmd, vars);
// 	// expand_redirections(curr_node->cmd.input, curr_node->cmd.output, vars);
// 	// set_redirections(curr_node->cmd.input, curr_node->cmd.output);
// }

char	**create_cmd_array(t_dlst *args)
{
	char	**cmd_array;
	int		lst_len;
	int		i;

	i = 0;
	lst_len = ft_dlstsize(args);
	cmd_array = ft_calloc(lst_len + 1, sizeof(char *));
	if (cmd_array == NULL)
		return (NULL);
	while (args != NULL)
	{
		cmd_array[i] = ft_strdup(get_toklist_str(args));
		if (cmd_array[i] == NULL)
		{
			free_array(cmd_array);
			return (NULL);
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

	// expand_args(curr_node->cmd.cmd, vars);
	// expand_redirections(curr_node->cmd.input, curr_node->cmd.output, vars);
	child_pid = FAILURE;
	builtin = is_builtin(curr_node);
	if (builtin)
		return (prepare_builtin_exec(builtin, curr_node, vars));
	paths_array = create_paths_array(vars, error);
	if (*error != success)
		return (*error);
	if (curr_node->parent && curr_node->parent->operator == pipeline)
		prepare_to_exec(curr_node, paths_array, vars);
	child_pid = fork();
	if (child_pid == FAILURE)
		*error = recoverable;
	if (child_pid == CHILD)
		prepare_to_exec(curr_node, paths_array, vars);
	free_array(paths_array);
	if (*error == success && child_pid != CHILD)
		return (wait_child());
	return (*error);
}


static int	exec_cmd(char *cmd_name, char **array2, t_shell_vars *vars)
{
	if (cmd_name != NULL)
		execve(cmd_name, array2, vars->env);
	else
		return (SUCCESS);
	if (errno == ENOEXEC || errno == EACCES)
		return (126);
	if (errno == ENOENT)
		return (127);
	else
		return (1);
}

int	exec_relative_path_cmd(char **paths_array, char **cmd_array, t_shell_vars *vars, t_bin_tree *curr_node)
{
	int		i;
	int		exit_value;
	char	*temp_line;

	i = 0;
	while (paths_array[i] != NULL)
	{
		temp_line = free_strjoin(paths_array[i], cmd_array[0], true, false);
		if (temp_line == NULL)
			return (CRIT_ERROR);
		paths_array[i] = temp_line;
		if (access(paths_array[i], F_OK) == 0)
			return (exec_cmd(paths_array[i], cmd_array, vars));
		i++;
	}
	exit_value = print_cmd_exec_issue(cmd_array[0], NULL, ": command not found\n", 127);
	free_arrays_tree_and_vars(paths_array, cmd_array, curr_node, vars);
	exit(exit_value);
}

void	prepare_to_exec(t_bin_tree *curr_node, char **paths_array, t_shell_vars *vars)
{
	int		exit_value;
	char	**cmd_array;

	exit_value = set_io_fds(curr_node);
	if (exit_value == ERROR)
	{
		free_array(paths_array);
		free_tree_and_vars(tree_root(curr_node), vars);
		exit(ERROR);
	}
	cmd_array = create_cmd_array(curr_node->content->tokens_list);
	if (cmd_array == NULL)
		exit_value = CRIT_ERROR;
	else if (paths_array != NULL && paths_array[0] != NULL
		&& cmd_array[0] != NULL
		&& ft_strnstr(cmd_array[0], "/", ft_strlen(cmd_array[0])) == NULL)
		exit_value = exec_relative_path_cmd(paths_array, cmd_array, vars, curr_node);
	else if (cmd_array != NULL)
		exit_value = exec_cmd(cmd_array[0], cmd_array, vars);
	if (exit_value == CRIT_ERROR)
		exit_value = print_exec_error(NULL, exit_value);
	else
		exit_value = print_exec_error(cmd_array[0], exit_value);
	free_arrays_tree_and_vars(paths_array, cmd_array, curr_node, vars);
	exit(exit_value);
}
