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

#define PIPE 1
#define OR 2
#define AND 3
#define CHILD 0
#define READ 0
#define WRITE 1
#define LEFT 1
#define RIGHT 2

// typedef	struct s_cmd
// {
// 	char	**cmd;
// 	char	*input;
// 	char	*output;
// }			t_cmd;

// typedef	struct s_node
// {
// 	struct s_node	*prev;
// 	struct s_node	*left;
// 	struct s_node	*right;
// 	char			op;
// 	char			**cmd;
// 	char			*name;
// }				t_node;

typedef	struct s_pipes_data
{
	int		pipe1[2];
	int		pipe2[2];
	char	to_close;
	char	is_init;
}				t_pipes_data;

/*CLEANING UTILS*/
int		close_pipe_perror(char *err_msg, int return_value, int *pipe);
int		close_pipe(int *pipe);

/*ENV UTILS*/
char	**create_paths_array(t_shell_vars *vars);

/*EXECUTION UTILS*/
void	execution(t_bin_tree *curr_node, char **paths_array, t_shell_vars *vars);
int		exec_cmd_tree(t_bin_tree *curr_node, t_shell_vars *vars);
int		prepare_to_exec(t_bin_tree *curr_node, t_shell_vars *vars);

/*PIPELINE HANDLING*/
int	fork_pipeline_sides(t_bin_tree *curr_node, t_shell_vars *vars);

/*SUBPROCESS UTILS*/
int	wait_childs(int second_child_pid);
int	get_exit_code(int child_exit_status);

/*TEST AND DEBUG*/
t_bin_tree *hardcoded_tree(void);

#endif
