/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   read.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: juportie <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/14 08:24:40 by juportie          #+#    #+#             */
/*   Updated: 2025/04/14 10:12:09 by juportie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>
#include <stdlib.h>
#include <readline/readline.h>
#include <readline/history.h>
#include "line_interaction.h"

void	input_loop(void)
{
	char	*line;

	while (1)
	{
		line = readline(PROMPT);
		if (strncmp(line, "exit", 4) == 0)
			break ;
		printf("%s\n", line);
		free(line);
	}
	free(line);
}

int	main(void)
{
	input_loop();
	rl_clear_history();
	return (EXIT_SUCCESS);
}
