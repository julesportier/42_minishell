/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expansions_wildcard_identity_check.c               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: juportie <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/26 10:25:15 by juportie          #+#    #+#             */
/*   Updated: 2025/06/09 17:46:05 by juportie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../libft/src/libft.h"
#include "../parsing/parsing.h"

t_bool	is_out_of_cwd(char *wildcard_str)
{
	int	i;

	i = 0;
	while (wildcard_str[i] == '.' && wildcard_str[i + 1] == '/')
	{
		++i;
		while (wildcard_str[i] == '/')
			++i;
	}
	while (wildcard_str[i])
	{
		if (wildcard_str[i] == '/')
		{
			while (wildcard_str[i] == '/')
				++i;
			if (wildcard_str[i] != '\0')
				return (true);
		}
		else
			++i;
	}
	return (false);
}

t_bool	is_matching_directories(char *wildcard_str)
{
	if (wildcard_str[ft_strlen(wildcard_str) - 1] == '/')
		return (true);
	else
		return (false);
}

t_bool	is_matching_hidden_files(char *wildcard_str)
{
	int	i;

	i = 0;
	while (wildcard_str[i] == '.' && wildcard_str[i + 1] == '/')
	{
		++i;
		while (wildcard_str[i] == '/')
			++i;
	}
	if (wildcard_str[i] == '.')
		return (true);
	else
		return (false);
}

t_bool	is_hidden_file(t_dlst *token)
{
	return (get_toklist_str(token)[0] == '.');
}

t_bool	is_non_hidden_file(t_dlst *token)
{
	return (get_toklist_str(token)[0] != '.');
}
