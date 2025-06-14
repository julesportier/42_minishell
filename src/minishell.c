/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ecasalin <ecasalin@42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/21 15:31:21 by juportie          #+#    #+#             */
/*   Updated: 2025/06/12 09:47:10 by ecasalin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include <readline/readline.h>
#include <readline/history.h>
#include "minishell.h"
#include "shell_init/init.h"
#include "cleaning_utils/cleaning.h"
#include "signals_utils/signals_utils.h"
#include "input/input.h"

volatile __sig_atomic_t	g_sig = 0;

int	main(int argc, char *argv[], char *envp[])
{
	t_shell_vars	vars;
	t_error			error;

	if (!isatty(STDIN_FILENO))
		return (EXIT_SUCCESS);
	if (argc != 1)
		return (2);
	(void)argv;
	error = success;
	if (init_shell_vars(&vars, envp, &error) == CRIT_ERROR)
		return (EXIT_FAILURE);
	if (init_input_sigaction() == -1)
		return (EXIT_FAILURE);
	input_loop(&vars, &error);
	free_array(vars.env);
	free(vars.cwd_backup);
	rl_clear_history();
	if (error == critical)
		return (EXIT_FAILURE);
	write(1, "exit\n", 5);
	return (vars.last_cmd_ext_code);
}
