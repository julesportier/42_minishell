/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   error_utils_1.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ecasalin <ecasalin@42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/25 16:35:53 by ecasalin          #+#    #+#             */
/*   Updated: 2025/06/16 12:29:08 by juportie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <unistd.h>
#include <stdio.h>
#include "../../libft/src/libft.h"
#include "../minishell.h"

int	return_perror(char *err_msg, int return_value)
{
	perror(err_msg);
	return (return_value);
}

t_error	return_perror_set_err(char *err_msg, t_error *error, t_error err_value)
{
	perror(err_msg);
	if (error)
		*error = err_value;
	return (err_value);
}

void	*return_perror_set_err_null(
	char *err_msg,
	t_error *error,
	t_error err_value)
{
	perror(err_msg);
	if (error)
		*error = err_value;
	return (NULL);
}

int	return_error(char *err_msg, int return_value)
{
	write(2, err_msg, ft_strlen(err_msg));
	return (return_value);
}

int	exit_perror(char *err_msg, int exit_value)
{
	perror(err_msg);
	exit(exit_value);
}
