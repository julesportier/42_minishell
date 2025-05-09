/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   error_print.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: juportie <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/06 17:29:09 by juportie          #+#    #+#             */
/*   Updated: 2025/05/06 17:30:22 by juportie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../libft/src/libft.h"
#include "../minishell.h" // TODO: FIX T_BOOL INCLUSION IN UTILS.H TO REMOVE THIS HEADER
#include "../general_utils/utils.h"
#include "parsing.h"

// If malloc fails better not crash the full shell
int	print_syntax_error(char *message, enum e_token_type type, int errnum)
{
	char *error;

	error = ft_strjoin("minishell: syntax error: ", message);
	if (error == NULL)
		return (errnum);
	error = free_strjoin(error, (char *)token_type_to_str(type), true, false);
	if (error == NULL)
		return (errnum);
	ft_putendl_fd(error, 2);
	free(error);
	return (errnum);
}
