/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ecasalin <ecasalin@42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/25 18:41:28 by kura              #+#    #+#             */
/*   Updated: 2025/05/01 17:44:09 by ecasalin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef BUILTINS_H
# define BUILTINS_H

/*BUILTINS*/
int	ms_echo(char **args);
int	ms_export(char **args, t_shell_vars *vars);

/*BUILTINS UTILS*/
char	*get_env_var(char *var_name, char **env);
char	*get_env_var_value(char *var_name, char **env);

#endif
