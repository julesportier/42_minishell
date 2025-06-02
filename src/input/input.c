/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   input.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ecasalin <ecasalin@42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/20 14:50:31 by juportie          #+#    #+#             */
/*   Updated: 2025/06/02 18:08:50 by ecasalin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <readline/readline.h>
#include <readline/history.h>
#include <errno.h>
#include "../../libft/src/libft.h"
#include "../minishell.h"
#include "../parsing/parsing.h"
#include "../exec/exec.h"
#include "../error_handling/errors.h"
#include "../cleaning_utils/cleaning.h"
#include "input.h"
#include "../signals_utils/signals_utils.h"
#include <signal.h>

static void	print_signal_reception(void)
{
	if (g_sig == SIGQUIT)
		printf("Quit (core dumped)\n");
	else if (g_sig == SIGINT)
		printf("\n");
}

static t_bin_tree	*parse_command_line(char *line, t_error *error)
{
	t_dlst		*toklist;
	t_bin_tree	*parse_tree;

	toklist = scan_line(line, error);
	free(line);
	if (*error == critical)
		return (NULL);
	if (toklist)
		return (parse_tree = build_parse_tree(&toklist, error));
	else
		return (NULL);
}

static void	switch_to_exec_mode(t_bin_tree *parse_tree, t_shell_vars *vars, t_error *error)
{
	init_exec_sigaction();
	exec_command_tree(parse_tree, vars, error);
	init_input_sigaction();
}

static void	reset_flag_vars(t_error *error)
{
	*error = success;
	g_sig = 0;
	errno = SUCCESS;
}

void	input_loop(t_shell_vars *vars, t_error *error)
{
	char		*line;
	t_bin_tree	*parse_tree;

	while (1)
	{
		reset_flag_vars(error);
		create_prompt(vars, PROMPT, error);
		if (*error == critical)
			break ;
		line = set_readline_and_history(vars->prompt, error);
		if (line == NULL)
			break ;
		parse_tree = parse_command_line(line, error);
		if (*error == critical)
			break ;
		if (parse_tree)
			switch_to_exec_mode(parse_tree, vars, error);
		free_tree(&parse_tree);
		if (*error == critical)
			break ;
		print_signal_reception();
		free(vars->prompt);
	}
		free(vars->prompt);
}
