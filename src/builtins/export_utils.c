/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ecasalin <ecasalin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/01 19:03:09 by ecasalin          #+#    #+#             */
/*   Updated: 2025/05/15 14:32:37 by ecasalin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include <unistd.h>
#include "../../libft/src/libft.h"
#include "../general_utils/utils.h"
#include "../minishell.h"

char	*find_biggest_env_var(char **env)
{
	int		i;
	char	*biggest_var;

	if (env == NULL)
		return (NULL);
	i = 0;
	biggest_var = env[0];
	while (env[i])
	{
		if (compare_env_var_names(env[i],
				biggest_var, get_var_name_len(env[i])) > 0)
			biggest_var = env[i];
		i++;
	}
	return (biggest_var);
}

char	*found_next_smallest_var(char *smaller_var, char **env)
{
	char	*var_to_print;
	int		i;
	int		var_name_len;

	var_to_print = find_biggest_env_var(env);
	i = 0;
	while (env[i])
	{
		var_name_len = get_var_name_len(env[i]);
		if (compare_env_var_names(env[i], smaller_var, var_name_len) > 0
			&& compare_env_var_names(env[i], var_to_print, var_name_len) < 0)
			var_to_print = env[i];
		i++;
	}
	return (var_to_print);
}

int	print_no_value_var(char *var)
{
	char	*var_to_print;
	int		var_len;
	int		temp;

	var_len = ft_strlen(var);
	var_to_print = ft_calloc(var_len + 2, 1);
	ft_memcpy(var_to_print, var, var_len);
	var_to_print[var_len] = '\n';
	temp = write(1, var_to_print, var_len + 1);
	free(var_to_print);
	if (temp == FAILURE)
		return (ERROR);
	return (SUCCESS);
}

int	print_quoted_env_var(char *var)
{
	char	*var;
	int		var_name_len;
	int		temp;

	if (!is_var_value_present(var))
		return (print_no_value_var(var));
	var_name_len = get_var_name_len(var);
	var = ft_calloc(ft_strlen(var) + 4, 1);
	if (var == NULL)
		return (CRIT_ERROR);
	ft_memcpy(var, var, var_name_len + 1);
	var = free_strjoin(var, "\"", true, false);
	if (var == NULL)
		return (CRIT_ERROR);
	var = free_strjoin(var, &var[var_name_len + 1], true, false);
	if (var == NULL)
		return (CRIT_ERROR);
	var = free_strjoin(var, "\"\n", true, false);
	if (var == NULL)
		return (CRIT_ERROR);
	temp = write(1, var, ft_strlen(var));
	free(var);
	if (temp == FAILURE)
		return (ERROR);
	return (SUCCESS);
}

int	print_sorted_env(char **env)
{
	char	*smallest_var;
	char	*var_to_print;
	int		y;
	int		error;

	y = count_array_len(env);
	smallest_var = "\0";
	while (y > 0)
	{
		var_to_print = found_next_smallest_var(smallest_var, env);
		error = print_quoted_env_var(var_to_print);
		if (error)
			return (error);
		smallest_var = var_to_print;
		y--;
	}
	return (SUCCESS);
}
