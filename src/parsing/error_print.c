/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   error_print.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: juportie <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/06 17:29:09 by juportie          #+#    #+#             */
/*   Updated: 2025/05/22 11:25:15 by juportie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>
#include "../../libft/src/libft.h"
#include "../general_utils/utils.h"
#include "parsing.h"

t_error	print_syntax_error(
	char *message,
	enum e_token_type type,
	t_error errnum,
	t_error *error)
{
	char	*error_str;

	if (error)
		*error = errnum;
	error_str = ft_strjoin("minishell: syntax error: `", message);
	if (error_str == NULL)
		return (err_print_alloc(critical, error));
	error_str = free_strjoin(
			error_str, (char *)token_type_to_str(type), true, false);
	if (error_str == NULL)
		return (err_print_alloc(critical, error));
	error_str = free_strjoin(error_str, "'\n", true, false);
	if (error_str == NULL)
		return (err_print_alloc(critical, error));
	ft_putstr_fd(error_str, 2);
	free(error_str);
	return (errnum);
}

t_error	err_perror_alloc(t_error errnum, t_error *error)
{
	if (error)
		*error = errnum;
	perror("minishell: parsing: critical error");
	return (errnum);
}

void	*null_perror_alloc(t_error errnum, t_error *error)
{
	if (error)
		*error = errnum;
	perror("minishell: parsing: critical error");
	return (NULL);
}

t_error	err_print_alloc(t_error errnum, t_error *error)
{
	if (error)
		*error = errnum;
	ft_putstr_fd("minishell: parsing: critical error: alloc failed\n", 2);
	return (errnum);
}

void	*null_print_alloc(t_error errnum, t_error *error)
{
	if (error)
		*error = errnum;
	ft_putstr_fd("minishell: parsing: critical error: alloc failed\n", 2);
	return (NULL);
}
