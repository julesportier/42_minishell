/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cleaning_utils.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kura <kura@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/25 17:57:36 by kura              #+#    #+#             */
/*   Updated: 2025/04/25 18:27:45 by kura             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include "../minishell.h"

void	*free_array(char **array)
{
	int	i;

	i = 0;
	if (array == NULL)
		return (NULL);
	while (array[i])
	{
		free(array[i]);
		i++;
	}
	free(array);
}

int	close_fd(int *fd)
{
	if (close(*fd) == -1)
	{
		perror("close_fd, close error");
		return (0);
	}
	*fd = -1;
	return (1);
}

int	close_pipe(int *pipe)
{
	int	temp1;
	int	temp2;

	temp1 = close(pipe[READ]);
	temp2 = close(pipe[WRITE]);
	if (temp1 == FAILURE || temp2 == FAILURE)
		return (FAILURE);
	return (SUCCESS);
}

int	close_pipe_perror(char *err_msg, int return_value, int *pipe)
{
	if (err_msg)
		perror(err_msg);
	close_pipe(pipe);
	return (return_value);
}
