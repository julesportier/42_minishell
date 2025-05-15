/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ecasalin <ecasalin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/25 17:00:23 by kura              #+#    #+#             */
/*   Updated: 2025/05/12 15:10:12 by ecasalin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef UTILS_H
# define UTILS_H

#include "../minishell.h"

/*GENERAL UTILS*/
int		count_array_len(char **array);
char	*free_strjoin(char *s1, char *s2, t_bool free_s1, t_bool free_s2);

/*ENV UTILS*/
int		is_var_value_present(char *var);
int		compare_env_var_names(char *var1, char *var2, int var1_name_len);
int		get_var_name_len(char *var);
char	*get_env_var(char *var_name, char **env);
char	*get_env_var_value(char *var_name, char **env);

/*PATHS UTILS*/
int		add_slash_to_paths(char **array);

#endif
