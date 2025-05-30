/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   error_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ecasalin <ecasalin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/25 16:35:53 by ecasalin          #+#    #+#             */
/*   Updated: 2025/05/19 11:08:36 by ecasalin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <unistd.h>
#include <stdio.h>
#include "../../libft/src/libft.h"
#include "../minishell.h"

void *set_err_return_null(t_error *error, t_error err_value)
{
	*error = err_value;
	return (NULL);
}

int	set_err_return_err(t_error *error, t_error err_value)
{
	*error = err_value;
	return (err_value);
}

int	return_perror(char *err_msg, int return_value)
{
	perror(err_msg);
	return (return_value);
}

int	return_perror_set_err(char *err_msg, t_error *error, t_error err_value)
{
	perror(err_msg);
	*error = err_value;
	return (err_value);
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
