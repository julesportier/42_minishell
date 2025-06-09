/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   input_loop_utils.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ecasalin <ecasalin@42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/02 15:11:52 by ecasalin          #+#    #+#             */
/*   Updated: 2025/06/09 10:52:08 by ecasalin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>
#include <readline/readline.h>
#include <readline/history.h>
#include "../../libft/src/libft.h"
#include "../minishell.h"
#include <errno.h>
#include "../error_handling/errors.h"
#include "input.h"

char	*set_readline_and_history(char *prompt, t_error *error)
{
	char	*line;

	line = readline(prompt);
	if (line == NULL)
	{
		if (errno == ENOMEM)
		{
			perror("minishell: readline");
			return (set_err_return_null(error, critical));
		}
		else
			return (NULL);
	}
	if (line[0] != '\0')
		add_history(line);
	return (line);
}

void	create_prompt(t_shell_vars *vars, t_error *error)
{
	if (vars->last_cmd_ext_code == SUCCESS)
		vars->prompt = ft_strjoin(vars->cwd_backup, PROMPT_SUCCESS);
	else
		vars->prompt = ft_strjoin(vars->cwd_backup, PROMPT_FAILURE);
	if (vars->prompt == NULL)
	{
		perror("minishell: prompt creation");
		*error = critical;
	}
}
