/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_env_array.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kura <kura@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/24 08:01:52 by ecasalin          #+#    #+#             */
/*   Updated: 2025/04/25 17:12:13 by kura             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <unistd.h>
#include "../../libft/src/libft.h"
#include "../error_handling/errors.h"
#include "../general_utils/utils.h"
#include "../minishell.h"

char	**init_env_array(char **env, t_error *error)
{
	char	**env_array;
	int		i;

	*error = success;
	i = count_array_len(env);
	if (env == NULL)
		return (NULL);
	env_array = malloc((i + 1) * sizeof(char *));
	if (env_array == NULL)
		return (set_err_return_null(error, critical));
	env_array[i] = NULL;
	i--;
	while (i >= 0)
	{
		env_array[i] = ft_strdup(env[i]);
		if (env_array[i] == NULL)
		{
			free_array(env_array);
			return (set_err_return_null(error, critical));
		}
		i--;
	}
	return (env_array);
}

// #include <stdio.h>
// int	main(int ac, char *av[], char *envp[])
// {
// 	char	**env_array;
// 	t_error	error;
// 	int		i;

// 	i = 0;
// 	env_array = init_env_array(envp, &error);
// 	if (env_array == NULL)
// 		return (printf("ENV = NULL"), 0);
// 	printf("ENVP:\n\n");
// 	while (envp[i])
// 	{
// 		printf("%s\n", envp[i]);
// 		i++;
// 	}
// 	// env_array = add_var_to_env(env_array, "VAR=TEST");
// 	// env_array = add_var_to_env(env_array, "VAR2=TEST2");
// 	// if (env_array == NULL)
// 	// 	return (printf("VAR_ADD: FAIL\n"));
// 	printf("\n\nENV_ARRAY:\n\n");
// 	i = 0;
// 	while (env_array[i])
// 	{
// 		printf("%s\n", env_array[i]);
// 		i++;
// 	}
// 	return (0);
// }

