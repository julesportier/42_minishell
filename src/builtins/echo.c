/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   echo.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kura <kura@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/23 07:07:22 by ecasalin          #+#    #+#             */
/*   Updated: 2025/04/26 12:16:58 by kura             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../error_handling/errors.h"
#include "../general_utils/utils.h"
#include <unistd.h>
#include "../../libft/src/libft.h"
#include "../minishell.h"

static int	is_valid_flag(char *arg)
{
	int	i;

	i = 1;
	if (arg[0] != '-' || !arg[1])
		return (0);
	while (arg[i])
	{
		if (arg[i] != 'n')
			return (0);
		i++;
	}
	return (1);
}

static int	fill_buffer(char **args, int i, char **buffer, int n_flag)
{
	*buffer = NULL;
	while (args[i])
	{
		*buffer = free_strjoin(*buffer, args[i]);
		if (*buffer == NULL)
			return (CRIT_ERROR);
		if (args[i + 1])
			*buffer = free_strjoin(*buffer, " ");
		if (*buffer == NULL)
			return (CRIT_ERROR);
		i++;
	}
	if (!n_flag)
	{
		*buffer = free_strjoin(*buffer, "\n");
		if (*buffer == NULL)
			return (CRIT_ERROR);
	}
	return (SUCCESS);
}

static int	write_and_free_buffer(char *buffer)
{
	int	temp;

	temp = write(1, buffer, ft_strlen(buffer));
	free(buffer);
	if (temp == FAILURE)
		return (return_perror("bash: echo", ERROR));
	return (SUCCESS);
}

int	ms_echo(char **args)
{
	int		n_flag;
	int		i;
	char	*buffer;

	i = 1;
	n_flag = 0;
	while (args[i])
	{
		if (is_valid_flag(args[i]))
		{
			n_flag = 1;
			i++;
		}
		else
			break;
	}
	if (fill_buffer(args, i, &buffer, n_flag) == CRIT_ERROR)
		return (return_perror("bash: echo", CRIT_ERROR));
	return (write_and_free_buffer(buffer));
}

// #include <stdio.h>
// int	main(int ac, char *av[])
// {
// 	int	return_val;

// 	return_val = ms_echo(&av[1]);
// 	printf("\n%d\n", return_val);
// }
