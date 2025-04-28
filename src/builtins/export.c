/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ecasalin <ecasalin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/25 15:41:16 by kura              #+#    #+#             */
/*   Updated: 2025/04/28 16:53:27 by ecasalin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <unistd.h>
#include "../../libft/src/libft.h"
#include "../error_handling/errors.h"
#include "../general_utils/utils.h"

// /*FAUT PASSER LE = AUSSI*/
// char	*get_env_var(char *var_name, char **env)
// {
// 	int		i;
	
// 	if (env == NULL || var_name == NULL)
// 		return (NULL);
// 	i = 0;
// 	while (env[i])
// 	{
// 		if (ft_strncmp(env[i], var_name, ft_strlen(var_name)) == SUCCESS)
// 			return (env[i]);
// 		i++;
// 	}
// 	return (NULL);
// }

// char	*get_env_var_value(char *var_name, char **env)
// {
// 	char	*var_value;
// 	int		i;
	
// 	var_value = get_env_var(var_name, env);
// 	if (var_value == NULL)
// 		return (NULL);
// 	i = 0;
// 	while (var_value[i] != '=')
// 		i++;
// 	return (&var_value[i + 1]);
// }

// /*POUR RAJOUTER DU PIQUANT, SI VAR = NULL CA SEGFAULT*/
// static char	**add_var_to_env(char **env, char *var)
// {
// 	int		i;
// 	char	**new_env_array;

// 	i = count_array_len(env);
// 	new_env_array = malloc((i + 2) * sizeof(char *));
// 	if (new_env_array == NULL)
// 		return (NULL);
// 	i = 0;
// 	while (env[i])
// 	{
// 		new_env_array[i] = env[i];
// 		i++;
// 	}
// 	new_env_array[i] = ft_strdup(var);
// 	if (new_env_array[i] == NULL)
// 	{
// 		free_array(new_env_array);
// 		return (NULL);
// 	}
// 	new_env_array[i + 1] = NULL;
// 	free_array(env);
// 	return (new_env_array);
// }
int	print_sorted_env(char **env)
{
	char	*smaller_var;
	char	*var_to_print;
	int		i;
	int		y;
	int		array_len;

	array_len = count_array_len(env);
	y = 0;
	smaller_var = "\1";
	var_to_print = env[0];
	while (y < array_len)
	{
		i = 0;
		while (env[i])
		{
			if (ft_strncmp(env[i], smaller_var, 1000) > 0 && ft_strncmp(env[i], var_to_print, 1000) < 0)
				var_to_print = env[i];
			i++;
		}
		y++;
		if (write(1, var_to_print, ft_strlen(var_to_print)) == FAILURE)
			return (return_perror("bash: export", ERROR));
		write(1, "\n", 1);
		smaller_var = var_to_print;
	}
	return (SUCCESS);
}

// int	ms_export(char **args, char **env)
// {
// 	int	i;
// 	int	temp;

// 	if (args[0] == NULL)
// 	{
// 		temp = print_sorted_env(env);
// 		if (temp == CRIT_ERROR)
// 			return (return_perror("bash: export critical error", CRIT_ERROR));
// 		if (temp == ERROR)
// 			return (return_perror("bash export", ERROR));
// 		return (SUCCESS);
// 	}
// 	i = 0;
// 	while (args[i])
// 	{
// 		env = add_var_to_env(env, args[i]);
// 		if (env == NULL)
// 			return (return_perror("bash: export critical error", CRIT_ERROR));
// 		i++;
// 	}
// 	return (SUCCESS);
// }

int	main(int ac, char *av[], char *envp[])
{
	print_sorted_env(envp);
}