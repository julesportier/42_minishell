/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expansions_wildcard_junk_utils.c                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: juportie <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/07 10:35:33 by juportie          #+#    #+#             */
/*   Updated: 2025/06/09 17:42:25 by juportie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../libft/src/libft.h"
#include "../minishell.h"
#include "../error_handling/errors.h"

size_t	get_junk_start_len(char *wildcard_str)
{
	size_t	i;

	i = 0;
	while (wildcard_str[i] == '.' && wildcard_str[i + 1] == '/')
	{
		++i;
		while (wildcard_str[i] == '/')
			++i;
	}
	return (i);
}

size_t	get_junk_end_len(char *wildcard_str)
{
	size_t	len;
	size_t	i;

	i = 0;
	len = ft_strlen(wildcard_str);
	while (len)
	{
		if (wildcard_str[len - 1] == '/')
			++i;
		else
			break ;
		--len;
	}
	return (i);
}

char	*trim_wildcard_str(char *wildcard_str, t_error *error)
{
	char	*str;
	
	str = ft_strdup(wildcard_str + get_junk_start_len(wildcard_str));
	if (!str)
		set_err_return_null(error, critical);
	str[ft_strlen(str) - get_junk_end_len(str)] = '\0';
	return (str);
}
