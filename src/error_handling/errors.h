/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   errors.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ecasalin <ecasalin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/25 16:47:33 by ecasalin          #+#    #+#             */
/*   Updated: 2025/06/17 08:33:23 by ecasalin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef ERRORS_H
# define ERRORS_H

# include "../minishell.h"

void	*set_err_return_null(t_error *error, t_error err_value);
int		return_perror(char *err_msg, int return_value);
int		return_error(char *err_msg, int return_value);
int		exit_perror(char *err_msg, int exit_value);
int		set_err_return_err(t_error *error, t_error err_value);
t_error	set_err_return_err_enun(t_error *error, t_error err_value);
t_error	return_perror_set_err(char *err_msg, t_error *error, t_error err_value);
void	*return_perror_set_err_null(
			char *err_msg,
			t_error *error,
			t_error err_value);
void	*null_print_alloc_err(t_error errnum, t_error *error);
int		perror_set_err(char *err_msg, t_error *error, t_error err_value);

#endif
