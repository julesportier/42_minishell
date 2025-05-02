/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   errors.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ecasalin <ecasalin@42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/25 16:47:33 by ecasalin          #+#    #+#             */
/*   Updated: 2025/05/02 14:25:39 by ecasalin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef ERRORS_H
# define ERRORS_H

# include "../minishell.h"

void	*set_err_return_null(t_error *error, t_error err_value);
int		return_perror(char *err_msg, int return_value);
int		return_error(char *err_msg, int return_value);

#endif
