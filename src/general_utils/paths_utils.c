/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   paths_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ecasalin <ecasalin@42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/09 09:18:40 by ecasalin          #+#    #+#             */
/*   Updated: 2025/06/09 08:49:43 by ecasalin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../libft/src/libft.h"
#include <unistd.h>
#include "../minishell.h"

char	get_last_char(char *str)
{
	if (str == NULL)
		return ('\0');
	return (str[ft_strlen(str) - 1]);
}

int	add_slash_to_paths(char **array)
{
	int		i;
	char	*slashed_line;
	char	line_last_char;

	i = 0;
	while (array[i] != NULL)
	{
		line_last_char = get_last_char(array[i]);
		if (line_last_char != '\0' && line_last_char != '/')
		{
			slashed_line = ft_strjoin(array[i], "/");
			if (slashed_line == NULL)
				return (CRIT_ERROR);
			free(array[i]);
			array[i] = slashed_line;
		}
		i++;
	}
	return (SUCCESS);
}
