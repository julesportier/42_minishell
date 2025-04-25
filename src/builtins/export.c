/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kura <kura@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/25 15:41:16 by kura              #+#    #+#             */
/*   Updated: 2025/04/25 17:49:21 by kura             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <unistd.h>
#include "../../libft/src/libft.h"
#include "../error_handling/errors.h"
#include "../general_utils/utils.h"

/*POUR RAJOUTER DU PIQUANT, SI VAR = NULL CA SEGFAULT*/
static char	**add_var_to_env(char **env, char *var)
{
	int		i;
	char	**new_env_array;

	i = count_array_len(env);
	new_env_array = malloc((i + 2) * sizeof(char *));
	if (new_env_array == NULL)
		return (NULL);
	i = 0;
	while (env[i])
	{
		new_env_array[i] = env[i];
		i++;
	}
	new_env_array[i] = ft_strdup(var);
	if (new_env_array[i] == NULL)
	{
		// free_array(new_env_array);
		return (NULL);
	}
	new_env_array[i + 1] = NULL;
	// free_array(env);
	return (new_env_array);
}
