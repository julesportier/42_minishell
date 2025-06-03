/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins_redir_utils.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ecasalin <ecasalin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/26 13:15:02 by ecasalin          #+#    #+#             */
/*   Updated: 2025/06/03 13:07:44 by ecasalin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <unistd.h>
#include <stdio.h>
#include "../minishell.h"
#include "../error_handling/errors.h"

int	save_shell_fds(int std_shell_fds[2])
{
	std_shell_fds[0] = dup(STDIN_FILENO);
	if (std_shell_fds[0] == FAILURE)
		return (return_perror("minishell: execution: redirection error", ERROR));
	std_shell_fds[1] = dup(STDOUT_FILENO);
	if (std_shell_fds[1] == FAILURE)
		return (return_perror("minishell: execution: redirection error", ERROR));
	return (SUCCESS);
}

int	restore_shell_fds(int std_shell_fds[2])
{
	int	return_value;
	int	temp1;
	int	temp2;

	return_value = SUCCESS;
	temp1 = dup2(std_shell_fds[0], STDIN_FILENO);
	temp2 = dup2(std_shell_fds[1], STDOUT_FILENO);
	if (temp1 == FAILURE || temp2 == FAILURE)
	{
		return_value = ERROR;
		perror("minishell: execution: redirection error");
	}
	close(std_shell_fds[0]);
	close(std_shell_fds[1]);
	return (return_value);
}
