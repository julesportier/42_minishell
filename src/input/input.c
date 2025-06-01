/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   input.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ecasalin <ecasalin@42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/20 14:50:31 by juportie          #+#    #+#             */
/*   Updated: 2025/06/01 16:58:12 by ecasalin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include <readline/readline.h>
#include <readline/history.h>
#include <errno.h>
#include "../../libft/src/libft.h"
#include "../minishell.h"
#include "../parsing/parsing.h"
#include "../exec/exec.h"
#include "../shell_init/init.h"
#include "../cleaning_utils/cleaning.h"
#include "input.h"
#include "../signals_utils/signals_utils.h"
#include <signal.h>

void	input_loop(t_shell_vars *vars, t_error *error)
{
	char	*line;
	t_dlst	*toklist;
	t_bin_tree	*parse_tree;

	while (1)
	{
		g_sig = 0;
		errno = SUCCESS;
		vars->prompt = ft_strjoin(vars->cwd_backup, PROMPT);
		if (vars->prompt == NULL)
			break ;
		line = readline(vars->prompt);
		if (line == NULL)
			break ;
		if (*line)
			add_history(line);
		toklist = scan_line(line, error);
		free(line);
		if (*error == critical)
			break ;
		// print_toklist(toklist, 0);
		if (toklist)
		{
			parse_tree = build_parse_tree(&toklist, error);
			if (*error == critical)
				break ;
			//print_tree(parse_tree, 0);
			if (!*error)
			{
				init_exec_sigaction();
				exec_cmd_tree(parse_tree, vars, error);
				init_input_sigaction();
			}
			free_tree(&parse_tree);
		}
		if (*error == critical)
			break ;
		*error = success;
		if (g_sig == SIGQUIT)
			printf("Quit (core dumped)\n");
		else if (g_sig == SIGINT)
			printf("\n");
		free(vars->prompt);
	}
		free(vars->prompt);
}
