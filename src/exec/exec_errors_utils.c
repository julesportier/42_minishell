/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_errors_utils.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ecasalin <ecasalin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/19 17:03:56 by ecasalin          #+#    #+#             */
/*   Updated: 2025/05/21 14:17:28 by ecasalin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include "../error_handling/errors.h"
#include "../general_utils/utils.h"
#include "../../libft/src/libft.h"
#include "../minishell.h"

int	print_cmd_exec_issue(char *cmd_name, char *msg, int return_value)
{
	char	*error_msg;

	error_msg = ft_strdup("minishell: ");
	if (error_msg == NULL)
		return (CRIT_ERROR);
	error_msg = free_strjoin(error_msg, cmd_name, true, false);
	if (error_msg == NULL)
		return (CRIT_ERROR);
	error_msg = free_strjoin(error_msg, msg, true, false);
	if (error_msg == NULL)
		return (CRIT_ERROR);
	ft_putstr_fd(error_msg, 2);
	free(error_msg);
	return (return_value);
}

static char *create_errno_msg(void)
{
	char	*formatted_strerror;

	formatted_strerror = ft_strdup(": ");
	if (formatted_strerror == NULL)
		return (NULL);
	formatted_strerror = free_strjoin(formatted_strerror, strerror(errno), true, false);
	if (formatted_strerror == NULL)
		return (NULL);
	formatted_strerror = free_strjoin(formatted_strerror, "\n", true, false);
	if (formatted_strerror == NULL)
		return (NULL);
	return (formatted_strerror);
}

int	print_exec_error(char *cmd_name, int exit_value)
{
	char	*formatted_strerror;

	if (exit_value == SUCCESS)
		return (SUCCESS);
	if (exit_value == CRIT_ERROR)
		return (return_perror("minishell: execution: critical error", ERROR));
	else
	{
		formatted_strerror = create_errno_msg();
		if (formatted_strerror == NULL)
			return (return_perror("minishell: execution: critical error", ERROR));
		exit_value = print_cmd_exec_issue(cmd_name, formatted_strerror, exit_value);
		free(formatted_strerror);
		return (exit_value);
	}
}
