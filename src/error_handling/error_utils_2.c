/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   error_utils_2.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ecasalin <ecasalin@42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/01 10:39:26 by ecasalin          #+#    #+#             */
/*   Updated: 2025/06/01 10:40:34 by ecasalin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <unistd.h>
#include "../minishell.h"

void *set_err_return_null(t_error *error, t_error err_value)
{
	*error = err_value;
	return (NULL);
}

int	set_err_return_err(t_error *error, t_error err_value)
{
	*error = err_value;
	return (ERROR);
}
