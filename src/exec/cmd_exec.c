/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd_exec.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ecasalin <ecasalin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: Invalid date        by                   #+#    #+#             */
/*   Updated: 2025/05/26 11:41:02 by ecasalin         ###   ########.fr       */
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

static void	free_arrays_tree_and_vars(char **paths_array, char **cmd_array, t_bin_tree *curr_node, t_shell_vars *vars)
{
	free_array(paths_array);
	free_array(cmd_array);
	free_tree_and_vars(tree_root(curr_node), vars);
}

char	**craft_cmd_array(t_dlst *args)
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

static int	reset_shell_fds(int std_shell_fds[2])
{
	int	return_value;
	
	return_value = SUCCESS;
	if (dup2(std_shell_fds[0], STDIN_FILENO) == FAILURE
		| dup2(std_shell_fds[1], STDOUT_FILENO) == FAILURE)
	{
		return_value = ERROR;
		perror("minishell: execution: redirection error");
	}
	close(std_shell_fds[0]);
	close(std_shell_fds[1]);
	return (return_value);
}

static int save_shell_fds(int std_shell_fds[2])
{
	std_shell_fds[0] = dup(STDIN_FILENO);
	if (std_shell_fds[0] == FAILURE)
		return (return_perror("minishell: execution: redirection error", ERROR));
	std_shell_fds[1] = dup(STDIN_FILENO);
	if (std_shell_fds[1] == FAILURE)
		return (return_perror("minishell: execution: redirection error", ERROR));
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
	cmd_array = craft_cmd_array(curr_node->content->tokens_list);
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
	cmd_array = craft_cmd_array(curr_node->content->tokens_list);
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
