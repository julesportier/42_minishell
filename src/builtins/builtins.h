/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ecasalin <ecasalin@42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/25 18:41:28 by ecasalin              #+#    #+#             */
/*   Updated: 2025/05/02 14:23:45 by ecasalin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef BUILTINS_H
# define BUILTINS_H

#include "../parsing/parsing.h"

typedef enum	e_builtin
{
	not_builtin,
	echo,
	cd,
	pwd,
	env,
	export,
	unset,
	ext
}				t_builtin;

typedef enum	e_exit_error
{
	no_error,
	not_a_digit,
	too_many_args,
	alloc_error,
}				t_exit_error;

typedef struct s_intf
{
	unsigned char	value;
	char			flag;
}					t_intf;

/*BUILTINS*/
int		ms_echo(char **args);
int		ms_export(char **args, t_shell_vars *vars);
int		ms_pwd(void);
int		ms_cd(char **args, t_shell_vars *vars);
int		ms_env(char **env);
int		ms_unset(char **args, t_shell_vars *vars);
int		ms_exit(char **args, t_shell_vars *vars, t_error *error);

/*EXPORT UTILS*/
char	*find_biggest_env_var(char **env);
char	*found_next_smallest_var(char *smaller_var, char **env);
int		print_no_value_var(char *var);
int		print_quoted_env_var(char *var);
int		print_sorted_env(char **env);
int		print_var_name_error(char *var);
int		print_export_failure(int return_value);
int		update_var(char *var, char *new_var_value, char **env);
char	**add_var_to_env(char *var, char **env);

/*CD UTILS*/
int		test_possible_paths(char *path, t_shell_vars *vars);
int		set_wd_var(char *var_key, char *var_value, t_shell_vars *vars);
int		update_cwd_backup(char *new_path, t_shell_vars *vars);
char	*concat_path_and_cwd_backup(char *cwd_backup, char *path);
int		update_wd_vars(t_shell_vars *vars);
int		chdir_unlinked_cwd(char *path, t_shell_vars *vars);
int		is_path_syntax_valid(char *path);
int		chdir_update_wd_vars(char *path, t_shell_vars *vars);

#endif
