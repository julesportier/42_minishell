/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ecasalin <ecasalin@42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/25 18:41:28 by kura              #+#    #+#             */
/*   Updated: 2025/04/30 11:57:03 by ecasalin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef BUILTINS_H
# define BUILTINS_H

/*BUILTINS*/
int	ms_echo(char **args);
int	ms_export(char **args, char **env);

/*BUILTINS UTILS*/
char	*get_env_var(char *var_name, char **env);
char	*get_env_var_value(char *var_name, char **env);

#endif
