/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ecasalin <ecasalin@42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/15 16:18:24 by ecasalin          #+#    #+#             */
/*   Updated: 2025/06/09 08:45:56 by ecasalin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <unistd.h>
#include <stdio.h>
#include "../../libft/src/libft.h"
#include "../error_handling/errors.h"
#include "../minishell.h"

int	ms_env(char **env)
{
	int	i;

	if (env == NULL)
		return (SUCCESS);
	i = 0;
	while (env[i])
	{
		if (ft_strchr(env[i], '=') != NULL)
			if (printf("%s\n", env[i]) == FAILURE)
				return (return_perror("minishell: env", ERROR));
		i++;
	}
	return (SUCCESS);
}
