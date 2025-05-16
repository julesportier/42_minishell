/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ecasalin <ecasalin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/15 16:18:24 by ecasalin          #+#    #+#             */
/*   Updated: 2025/05/16 12:34:25 by ecasalin         ###   ########.fr       */
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

// #include "../shell_init/init.h"
// int	main(int ac, char *av[], char *envp[])
// {
// 	char	**env;
// 	t_error	error;

// 	env = init_env_array(envp, &error);
// 	return (ms_env(env));
// }