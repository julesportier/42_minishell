/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   general_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kura <kura@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/25 16:24:38 by kura              #+#    #+#             */
/*   Updated: 2025/04/25 17:05:49 by kura             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <unistd.h>
// #include "../libft/src/libft.h"
#include "../minishell.h"

int	count_array_len(char **array)
{
	int	i;

	i = 0;
	if (array == NULL)
		return (0);
	while (array[i])
		i++;
	return (i);
}
