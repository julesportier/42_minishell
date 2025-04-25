/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   error_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kura <kura@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/25 16:35:53 by kura              #+#    #+#             */
/*   Updated: 2025/04/25 16:48:59 by kura             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <unistd.h>
// #include "../../libft/src/libft.h"
#include "../minishell.h"

void *set_err_return_null(t_error *error, t_error err_value)
{
	*error = err_value;
	return (NULL);
}
