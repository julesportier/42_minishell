/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   errors.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kura <kura@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/25 16:47:33 by kura              #+#    #+#             */
/*   Updated: 2025/04/26 11:56:10 by kura             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef ERRORS_H
# define ERRORS_H

# include "../minishell.h"

void	*set_err_return_null(t_error *error, t_error err_value);
int		return_perror(char *err_msg, int return_value);
#endif
