/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   error_utils_3.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ecasalin <ecasalin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/25 16:35:53 by ecasalin          #+#    #+#             */
/*   Updated: 2025/06/17 08:33:14 by ecasalin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <unistd.h>
#include <stdio.h>
#include "../../libft/src/libft.h"
#include "../minishell.h"

void	*null_print_alloc_err(t_error errnum, t_error *error)
{
	if (error)
		*error = errnum;
	ft_putstr_fd("minishell: critical error: alloc failed\n", 2);
	return (NULL);
}

int	perror_set_err(char *err_msg, t_error *error, t_error err_value)
{
	perror(err_msg);
	if (error)
		*error = err_value;
	return (ERROR);
}
