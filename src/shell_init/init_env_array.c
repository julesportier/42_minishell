/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_env_array.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ecasalin <ecasalin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/24 08:01:52 by ecasalin          #+#    #+#             */
/*   Updated: 2025/04/24 11:19:59 by ecasalin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <unistd.h>
#include "../../libft/src/libft.h"
#include "../minishell.h"

static void *set_err_return_null(t_error *error, t_error err_value)
{
	*error = err_value;
	return (NULL);
}

char	**malloc_env_array(char **envp, t_error *error)
{
	char	**env_array;
	int		i;
	
	*error = success;
	i = 0;
	if (envp == NULL)
		return (NULL);
	while (envp[i])
		i++;
	env_array = malloc((i + 1) * sizeof(char *));
	if (env_array == NULL)
		return (set_err_return_null(error, critical));
	env_array[i] = NULL;
	i--;
	while (i >= 0)
	{
		env_array[i] = ft_strdup(envp[i]);
		if (env_array[i] == NULL)
		{
			free_array(env_array);
			return (set_err_return_null(error, critical));
		}
		i--;
	}
	return (env_array);
}

char	**add_var_to_env(t_shell_vars *vars)
{
	
}
// #include <stdio.h>
// int	main(int ac, char *av[], char *envp[])
// {
// 	char	**env_array;
// 	t_error	error;
// 	int		i;

// 	i = 0;
// 	env_array = malloc_env_array(envp, &error);
// 	if (env_array == NULL)
// 		return (printf("ENV = NULL"), 0);
// 	printf("ENVP:\n\n");
// 	while (envp[i])
// 	{
// 		printf("%s\n", envp[i]);
// 		i++;
// 	}
// 	printf("\n\nENV_ARRAY:\n\n");
// 	i = 0;
// 	while (env_array[i])
// 	{
// 		printf("%s\n", env_array[i]);
// 		i++;
// 	}
// 	return (0);
// }

