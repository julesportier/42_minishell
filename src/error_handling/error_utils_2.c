/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   error_utils_2.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ecasalin <ecasalin@42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/01 10:39:26 by ecasalin          #+#    #+#             */
/*   Updated: 2025/06/09 09:12:54 by ecasalin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <unistd.h>
#include "../../libft/src/libft.h"
#include "../minishell.h"
#include "../general_utils/utils.h"

void	*set_err_return_null(t_error *error, t_error err_value)
{
	*error = err_value;
	return (NULL);
}

int	set_err_return_err(t_error *error, t_error err_value)
{
	*error = err_value;
	return (ERROR);
}

t_error	set_err_return_err_enun(t_error *error, t_error err_value)
{
	*error = err_value;
	return (err_value);
}
