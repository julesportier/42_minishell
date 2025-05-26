/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ecasalin <ecasalin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/18 07:31:33 by ecasalin          #+#    #+#             */
/*   Updated: 2025/04/23 13:35:43 by ecasalin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef EXEC_H
# define EXEC_H

#include "../minishell.h"
#include "../parsing/parsing.h"
#include "../builtins/builtins.h"
#include <sys/types.h>

#define CHILD 0
#define READ 0
#define WRITE 1

/*CLEANING UTILS*/
int		close_pipe_perror(char *err_msg, int return_value, int *pipe);
int		close_pipe(int *pipe);
void	free_arrays_tree_and_vars(char **paths_array, char **cmd_array, t_bin_tree *curr_node, t_shell_vars *vars);

/*ERROR UTILS*/
int		print_cmd_exec_issue(char *cmd_name, char *arg, char *msg, int return_value);
int		print_exec_error(char *cmd_name, int exit_value);

/*ENV UTILS*/
char	**create_paths_array(t_shell_vars *vars, t_error *error);

/*EXECUTION UTILS*/
void	prepare_to_exec(t_bin_tree *curr_node, char **paths_array, t_shell_vars *vars);
char	**create_cmd_array(t_dlst *args);
int		exec_cmd_tree(t_bin_tree *curr_node, t_shell_vars *vars, t_error *error);
int		create_exec_setup(t_bin_tree *curr_node, t_shell_vars *vars, t_error *error);

/*PIPELINE HANDLING*/
int		fork_pipeline_sides(t_bin_tree *curr_node, t_shell_vars *vars, t_error *error);

/*SUBPROCESS UTILS*/
int		wait_2_children(pid_t second_child_pid);
int		get_exit_code(int child_exit_status);
int		fork_subshell(t_bin_tree *curr_node, t_shell_vars *vars, t_error *error);
int		wait_child(void);

/*BUILTINS HANDLING*/
int		is_builtin(t_bin_tree *curr_node);
int		exec_builtin(t_builtin builtin, char **cmd_array, t_shell_vars *vars, t_exit_error *exit_error);
int		prepare_builtin_exec(t_builtin builtin, t_bin_tree *curr_node, t_shell_vars *vars);

/*REDIRECTIONS HANDLING*/
int		set_input(t_bin_tree *curr_node);
int		set_output(t_bin_tree *curr_node);
int		set_io_fds(t_bin_tree *curr_node);
int		save_shell_fds(int std_shell_fds[2]);
int		reset_shell_fds(int std_shell_fds[2]);

/*TEST AND DEBUG*/
// t_bin_tree *hardcoded_tree(void);

#endif
