/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   error_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ecasalin <ecasalin@42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/25 16:35:53 by ecasalin          #+#    #+#             */
/*   Updated: 2025/05/12 21:58:38 by ecasalin         ###   ########.fr       */
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

int	return_perror(char *err_msg, int return_value)
{
	perror(err_msg);
	return (return_value);
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
