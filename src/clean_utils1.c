/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   clean_utils1.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ecasalin <ecasalin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/24 09:30:22 by erik              #+#    #+#             */
/*   Updated: 2025/04/18 07:39:50 by ecasalin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	*free_array(char **array, int i)
{
	int	temp_index;

	if (array == NULL)
		return (NULL);
	if (array[i] != NULL)
		free(array[i]);
	temp_index = i;
	if (i != 0)
	{
		temp_index--;
		while (temp_index >= 0 && array[temp_index] != NULL)
		{
			free(array[temp_index]);
			temp_index--;
		}
	}
	i++;
	while (array[i] != NULL)
	{
		free(array[i]);
		i++;
	}
	free(array);
	array = NULL;
	return (NULL);
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
