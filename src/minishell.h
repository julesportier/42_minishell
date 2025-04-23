/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ecasalin <ecasalin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/18 07:31:33 by ecasalin          #+#    #+#             */
/*   Updated: 2025/04/23 09:46:52 by ecasalin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../libft/src/libft.h"

#include <unistd.h>
#include <stdio.h>
#include <errno.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>

#define PIPE 1
#define OR 2
#define AND 3
#define CHILD 0
#define READ 0
#define WRITE 1
#define ERROR 1
#define SUCCESS 0
#define LEFT 1
#define RIGHT 2
#define INTERNAL_ERR -1
#define FAILURE -1
#define FATAL -2

typedef	struct s_cmd
{
	char	**cmd;
	char	*input;
	char	*output;
}			t_cmd;

typedef	struct s_node
{
	struct s_node	*prev;
	struct s_node	*left;
	struct s_node	*right;
	char			op;
	char			**cmd;
	char			*name;
}				t_node;

typedef	struct s_pipes_data
{
	int		pipe1[2];
	int		pipe2[2];
	char	to_close;
	char	is_init;
}				t_pipes_data;

typedef	struct s_var_data
{
	char	**env;
	int		last_cmd_ext_code;
}				t_var_data;

/*PRINTING_ERRORS*/
int		return_perror(char *err_msg, int return_value);

/*CLEANING UTILS*/
void	*free_array(char **array, int i);
int		close_pipe_perror(char *err_msg, int return_value, int *pipe);
int		close_pipe(int *pipe);

/*LIBFT FUNCTIONS VARIANTS*/
size_t	ft_nstrlen(const char *s);
char	*ft_fstrjoin(char *s1, char *s2);

/*ENV UTILS*/
char	**create_paths_array(t_var_data *vars);

/*EXECUTION UTILS*/
void	execution(t_node *curr_node, char **paths_array, t_var_data *vars);
int		exec_cmd_tree(t_node *curr_node, t_var_data *vars);
int		prepare_to_exec(t_node *curr_node, t_var_data *vars);

/*PIPELINE HANDLING*/
int	fork_pipeline_sides(t_node *curr_node, t_var_data *vars);

/*SUBPROCESS UTILS*/
int	wait_childs(int second_child_pid);
int	get_exit_code(int child_exit_status);

/*TEST AND DEBUG*/
t_node *hardcoded_tree(void);
