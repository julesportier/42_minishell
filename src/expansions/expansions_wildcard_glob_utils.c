/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expansions_wildcard_glob_utils.c                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: juportie <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/26 10:25:15 by juportie          #+#    #+#             */
/*   Updated: 2025/06/09 14:51:17 by juportie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <dirent.h>
#include "../minishell.h"

int	get_len_until_star(char *shifted_wildcard_str)
{
	int	i;

	i = 0;
	while (*(shifted_wildcard_str + i))
	{
		if (*(shifted_wildcard_str + i) == '*')
			return (i);
		++i;
	}
	return (i);
}

int	skip_wildcards(char *wildcard_str, int i, t_bool *after_wildcard)
{
	*after_wildcard = false;
	while (wildcard_str[i] == '*')
	{
		*after_wildcard = true;
		++i;
	}
	return (i);
}

void	increment_indexes(int *i, int *glob_index, char *shifted_wildcard_str)
{
	int	len;

	len = get_len_until_star(shifted_wildcard_str);
	*glob_index += len;
	*i += len;
}
