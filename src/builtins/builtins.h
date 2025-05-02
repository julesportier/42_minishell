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

/*BUILTINS*/
int	ms_echo(char **args);
int	ms_export(char **args, t_shell_vars *vars);
int	ms_pwd(void);

/*EXPORT UTILS*/
char	*find_biggest_env_var(char **env);
char	*found_next_smallest_var(char *smaller_var, char **env);
int		print_no_value_var(char *var);
int		print_quoted_env_var(char *var);
int		print_sorted_env(char **env);
int		print_var_name_error(char *var);
int		print_export_failure(int return_value);

#endif
