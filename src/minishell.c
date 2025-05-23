/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ecasalin <ecasalin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/21 15:31:21 by juportie          #+#    #+#             */
/*   Updated: 2025/05/23 11:01:29 by ecasalin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include <readline/readline.h>
#include <readline/history.h>
#include "minishell.h"
#include "shell_init/init.h"
#include "cleaning_utils/cleaning.h"
#include "input/input.h"

int	main(int argc, char *argv[], char *envp[])
{
	t_shell_vars	vars;
	t_error	error;

	if (argc != 1)
		return (2);
	(void)argv;
	error = success;
	vars.env = init_env_array(envp, &error);
	vars.cwd_backup = init_cwd_backup();
	vars.prompt = NULL;
	if (init_sigaction() == -1)
		return (EXIT_FAILURE);
	input_loop(&vars, &error);
	free_array(vars.env);
	free(vars.cwd_backup);
	rl_clear_history();
	if (error == critical)
		return (EXIT_FAILURE);
	return (EXIT_SUCCESS);
}
