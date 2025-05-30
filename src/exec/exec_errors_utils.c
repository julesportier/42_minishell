/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_errors_utils.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ecasalin <ecasalin@42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: Invalid date        by                   #+#    #+#             */
/*   Updated: 2025/05/30 22:49:38 by ecasalin         ###   ########.fr       */
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

int	free_array_set_err(t_error *error, t_error err_value, char **array)
{
	free_array(array);
	return (set_err_return_err(error, err_value));
}

int print_joined_cmd_error(char *cmd_name, char *arg, char *msg, t_error *error)
{
	char	*error_msg;

	error_msg = ft_strdup("minishell: ");
	if (error_msg == NULL)
		return (set_err_return_err(error, critical));
	error_msg = free_strjoin(error_msg, cmd_name, true, false);
	if (error_msg == NULL)
		return (set_err_return_err(error, critical));
	if (arg != NULL)
	{
		error_msg = free_strjoin(error_msg, arg, true, false);
		if (error_msg == NULL)
			return (set_err_return_err(error, critical));
	}
	error_msg = free_strjoin(error_msg, msg, true, false);
	if (error_msg == NULL)
		return (set_err_return_err(error, critical));
	ft_putstr_fd(error_msg, 2);
	free(error_msg);
	return (SUCCESS);
}

static char *create_errno_msg(t_error *error)
{
	char	*formatted_strerror;

	formatted_strerror = ft_strdup(": ");
	if (formatted_strerror == NULL)
		return (set_err_return_null(error, critical));
	formatted_strerror = free_strjoin(formatted_strerror, strerror(errno), true, false);
	if (formatted_strerror == NULL)
		return (set_err_return_null(error, critical));
	formatted_strerror = free_strjoin(formatted_strerror, "\n", true, false);
	if (formatted_strerror == NULL)
		return (set_err_return_null(error, critical));
	return (formatted_strerror);
}

int	print_exec_error(char *cmd_name, int return_value, t_error *error)
{
	char	*formatted_strerror;

	if (return_value == SUCCESS)
		return (SUCCESS);
	else
	{
		formatted_strerror = create_errno_msg(error);
		if (*error)
			return (return_perror("minishell: execution: critical error", ERROR));
		print_joined_cmd_error(cmd_name, NULL, formatted_strerror, error);
		free(formatted_strerror);
		if (*error)
			return (return_perror("minishell: execution: critical error", ERROR));
		return (return_value);
	}
}
