/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_clean_utils.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ecasalin <ecasalin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/24 09:30:22 by erik              #+#    #+#             */
/*   Updated: 2025/05/19 17:04:58 by ecasalin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include "../error_handling/errors.h"
#include "../general_utils/utils.h"
#include "exec.h"
#include "../minishell.h"

int	close_fd(int *fd) //pas utilisé pour l'instant
{
	if (close(*fd) == FAILURE)
		return_perror("minishell: execution: close error", ERROR);
	*fd = -1;
	return (SUCCESS);
}

int	close_pipe(int *pipe)
{
	int	temp1;
	int	temp2;

	temp1 = close(pipe[READ]);
	temp2 = close(pipe[WRITE]);
	if (temp1 == FAILURE || temp2 == FAILURE)
		return (return_perror("minishell: execution: error closing pipe", ERROR));
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
