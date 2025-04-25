/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   clean_utils1.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kura <kura@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/24 09:30:22 by erik              #+#    #+#             */
/*   Updated: 2025/04/25 18:17:48 by kura             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <unistd.h>
#include "minishell.h"

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

// int	close_pipe_fds(int *pipe)
// {
// 	if (pipe[READ_END] != -1)
// 		if (close(pipe[READ_END]) == 0)
// 			pipe[READ_END] = -1;
// 	if (pipe[WRITE_END] != -1)
// 		if (close(pipe[WRITE_END]) == 0)
// 			pipe[WRITE_END] = -1;
// 	if (pipe[READ_END] != -1 || pipe[WRITE_END] != -1)
// 		return (return_perror("close_pipe_fds, close error", 0));
// 	return (1);
// }

// void	cls_fds_free(t_data *data)
// {
// 	close_pipe_fds(data->p1);
// 	close_pipe_fds(data->p2);
// 	free_array(data->paths_array, 0);
// 	if (data->here_doc_fd != -1)
// 		close_fd(&data->here_doc_fd);
// }
