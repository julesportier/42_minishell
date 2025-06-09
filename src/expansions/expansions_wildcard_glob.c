/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expansions_wildcard_glob.c                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: juportie <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/26 10:25:15 by juportie          #+#    #+#             */
/*   Updated: 2025/06/09 17:53:02 by juportie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <dirent.h>
#include <stdio.h>
#include "../../libft/src/libft.h"
#include "expansions.h"

static t_bool	match_glob_substr(
	char *shifted_wildcard_str,
	char *shifted_entry)
{
	size_t	substr_len;

	substr_len = get_len_until_star(shifted_wildcard_str);
	if (substr_len > ft_strlen(shifted_entry))
		return (false);
	if (ft_strncmp(shifted_wildcard_str, shifted_entry, substr_len))
		return (false);
	else
		return (true);
}

static t_bool	match_glob_substr_after_wildcard(
	char *wildcard_str,
	char *entry,
	int *i,
	int *glob_index)
{
	while (entry[*glob_index])
	{
		if (match_glob_substr(&(wildcard_str[*i]), &(entry[*glob_index])))
		{
			increment_indexes(i, glob_index, &wildcard_str[*i]);
			return (true);
		}
		else
			++(*glob_index);
	}
	return (false);
}

t_bool	match_glob_entry(char *wildcard_str, char *entry)
{
	int		i;
	int		glob_index;
	t_bool	after_wildcard;

	i = 0;
	glob_index = 0;
	while (wildcard_str[i])
	{
		i = skip_wildcards(wildcard_str, i, &after_wildcard);
		if (!wildcard_str[i])
			break ;
		if (after_wildcard)
		{
			if (match_glob_substr_after_wildcard(
					wildcard_str, entry, &i, &glob_index))
				after_wildcard = false;
		}
		else if (match_glob_substr(&(wildcard_str[i]), &(entry[glob_index])))
			increment_indexes(&i, &glob_index, &wildcard_str[i]);
		else
			return (false);
	}
	if (entry[glob_index] && !after_wildcard)
		return (false);
	return (true);
}
