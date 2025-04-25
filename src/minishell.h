/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kura <kura@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/22 16:31:37 by juportie          #+#    #+#             */
/*   Updated: 2025/04/25 18:25:47 by kura             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

#define FAILURE -1
#define SUCCESS 0
#define ERROR 1
#define CRIT_ERROR 2
#define READ 0
#define WRITE 1

typedef enum	e_bool
{
	false = 0,
	true = 1
}	t_bool;

typedef enum	e_error
{
	success = 0,
	recoverable,
	critical
}	t_error;

typedef	struct s_shell_vars
{
	char	**env;
	int		last_cmd_ext_code;
}				t_shell_vars;

#endif
