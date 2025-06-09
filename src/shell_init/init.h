/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.h                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ecasalin <ecasalin@42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/25 17:15:10 by kura              #+#    #+#             */
/*   Updated: 2025/06/09 09:53:55 by ecasalin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef INIT_H
# define INIT_H

# include "../minishell.h"

char	**init_env_array(char **env, t_error *error);
char	*init_cwd_backup(void);
int		init_shell_vars(t_shell_vars *vars, char **envp, t_error *error);

#endif
