/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   input_loop_utils.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ecasalin <ecasalin@42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/02 15:11:52 by ecasalin          #+#    #+#             */
/*   Updated: 2025/06/09 09:50:32 by ecasalin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>
#include <readline/readline.h>
#include <readline/history.h>
#include "../../libft/src/libft.h"
#include "../minishell.h"
#include <errno.h>
#include "../error_handling/errors.h"

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

void	create_prompt(t_shell_vars *vars,
			char *personalized_prompt, t_error *error)
{
	vars->prompt = ft_strjoin(vars->cwd_backup, personalized_prompt);
	if (vars->prompt == NULL)
	{
		perror("minishell: prompt creation");
		*error = critical;
	}
}
