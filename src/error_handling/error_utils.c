/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   error_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kura <kura@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/25 16:35:53 by kura              #+#    #+#             */
/*   Updated: 2025/04/26 11:58:41 by kura             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <unistd.h>
#include <stdio.h>
// #include "../../libft/src/libft.h"
#include "../minishell.h"

void *set_err_return_null(t_error *error, t_error err_value)
{
	*error = err_value;
	return (NULL);
}

int		return_perror(char *err_msg, int return_value)
{
	perror(err_msg);
	return (return_value);
}
