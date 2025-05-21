/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   shell_init.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ecasalin <ecasalin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/24 08:01:52 by ecasalin          #+#    #+#             */
/*   Updated: 2025/05/21 13:56:57 by ecasalin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include "../../libft/src/libft.h"
#include "../error_handling/errors.h"
#include "../general_utils/utils.h"
#include "../cleaning_utils/cleaning.h"
#include "../minishell.h"
#include "../builtins/builtins.h"
#include <errno.h>

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
		env_array[i] = ft_strdup_s(env[i]);
		if (env_array[i] == NULL)
		{
			free_array(env_array);
			return (set_err_return_null(error, critical));
		}
		i--;
	}
	return (env_array);
}

/*Dans le main faudra bien free env et se barrer si ca return NULL*/
char	*init_cwd_backup(void)
{
	char		*cwd;
	
	cwd = getcwd(NULL, 0);
	if (errno == ENOMEM)
		return (NULL);
	if (cwd == NULL)
	{
		cwd = ft_strdup_s("/");
		if (cwd == NULL)
			return (NULL);
		ft_putstr_fd("minishell: current working directory is unlinked: setting internal backup cwd to /\n", 2);
		errno = 0;
	}
	return (cwd);
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
// 	printf("\n%s\n", get_env_var("GJS_DEBUG_TOPICS=", env_array));
// 	printf("%s\n", get_env_var_value("CDPATH=", env_array));
// 	return (0);
// }

