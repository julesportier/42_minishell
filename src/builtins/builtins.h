/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ecasalin <ecasalin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/25 18:41:28 by kura              #+#    #+#             */
/*   Updated: 2025/04/28 12:28:03 by ecasalin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef BUILTINS_H
# define BUILTINS_H

int	ms_echo(char **args);
/*TEMP*/
char	*get_env_var(char *var_name, char **env);
static char	**add_var_to_env(char **env, char *var);
char	*get_env_var_value(char *var_name, char **env);

#endif
