/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd_exec.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ecasalin <ecasalin@42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/28 15:00:16 by ecasalin          #+#    #+#             */
/*   Updated: 2025/05/18 16:56:41 by ecasalin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../libft/src/libft.h"
#include <unistd.h>
#include <stdio.h>
#include <sys/wait.h>
#include "../general_utils/utils.h"
#include "../error_handling/errors.h"
#include "../cleaning_utils/cleaning.h"
#include "../parsing/parsing.h"
#include "exec.h"
#include "../minishell.h"

// int	execution(t_node *curr_node, char **paths_array, t_shell_vars *vars)
// {
// 	// expand_args(curr_node->cmd.cmd, vars);
// 	// expand_redirections(curr_node->cmd.input, curr_node->cmd.output, vars);
// 	// set_redirections(curr_node->cmd.input, curr_node->cmd.output);
// }

int	prepare_to_exec(t_bin_tree *curr_node, t_shell_vars *vars)
{
	char	**paths_array;
	int		child_pid;
	int		child_exit_status;
	t_error	error;

	paths_array = create_paths_array(vars, &error);
	// if (is_builtin(curr_node->cmd))
		// return (exec_builtin(curr_node, vars));
	if (curr_node->parent && curr_node->parent->operator == PIPE)
		execution(curr_node, paths_array, vars);
	child_pid = fork();
	if (child_pid == CHILD)
		execution(curr_node, paths_array, vars);
	else
	{
		waitpid(child_pid, &child_exit_status, 0);
		free_array(paths_array);
		return (get_exit_code(child_exit_status));
	}
	free_array(paths_array);
	return (ERROR);
}

static void	exec_cmd(char **array1, char **array2, int i, t_shell_vars *vars)
{
	execve(array1[i], array2, vars->env);
	// if (errno == ENOEXEC || errno == EACCES)
	// 	exit_free_close(array1[i], 126, data);
	// if (errno == ENOENT)
	// 	exit_free_close(array1[i], 127, data);
	// else
	// 	exit_free_close(array1[i], 1, data);
	perror("EXEC_FAIL");
}

// static void	set_cmd_arg_array(t_data *data)
// {
// 	if (data->argv[data->cmd][0] == '\0' || data->argv[data->cmd][0] == ' ')
// 	{
// 		print_cmd_nfound(data);
// 		exit_free_close(NULL, 127, data);
// 	}
// 	data->cmd_arg_array = ft_split(data->argv[data->cmd], ' ');
// 	if (data->cmd_arg_array == NULL)
// 		exit_free_close("set_cmd_arg_array, malloc error", 1, data);
// }


/*NO PROTECTIONS AGAINST SEGFAULT*/
char	**craft_cmd_array(t_dlst *args)
{
	char	**cmd_array;
	int		lst_len;
	int		i;

	i = 0;
	lst_len = ft_dlstsize(args);
	cmd_array = calloc(lst_len + 1, sizeof(char *));
	while (args != NULL)
	{
		cmd_array[i] = ft_strdup(get_toklist_str(args));
		i++;
		args = args->next;
	}
	return (cmd_array);
}

void	execution(t_bin_tree *curr_node, char **paths_array, t_shell_vars *vars)
{
	int		i;
	char	**cmd_array;

	cmd_array = craft_cmd_array(curr_node->content->tokens_list);
	i = 0;
	if (paths_array != NULL && paths_array[i] != NULL &&
			ft_strnstr(cmd_array[0],
			"/", ft_strlen(cmd_array[0])) == NULL )
	{
		while (paths_array[i] != NULL)
		{
			paths_array[i] = free_strjoin(paths_array[i],
					cmd_array[0], true, false);
			// if (paths_array[i] == NULL)
			// 	exit_free_close("check_cmd_path, malloc error", 1, data);
			if (access(paths_array[i], F_OK) == 0)
				exec_cmd(paths_array, cmd_array, i, vars);
			i++;
		}
		perror("BAD PATH");
		// if (print_cmd_nfound(data) == 0)
		// 	exit_free_close(NULL, 1, data);
		// ft_putstr_fd(data->cmd_arg_array[0], 2);
		// exit_free_close(NULL, 127, data);
	}
	exec_cmd(cmd_array, cmd_array, 0, vars);
	free_tree_and_vars(tree_root(curr_node), vars);
}
