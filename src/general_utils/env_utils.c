/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_utils.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ecasalin <ecasalin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/01 18:52:27 by ecasalin          #+#    #+#             */
/*   Updated: 2025/05/21 13:56:14 by ecasalin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <unistd.h>
#include "../minishell.h"

int	is_var_value_present(char *var)
{
	int	i;

	i = 0;
	while (var[i])
	{
		if (var[i] == '=')
			return (1);
		i++;
	}
	return (0);
}

int	compare_env_var_names(char *var1, char *var2, int var1_name_len)
{
	int	i;

	i = 0;
	while (var1_name_len > 0)
	{
		if (var2[i] != var1[i])
			return (var1[i] - var2[i]);
		i++;
		var1_name_len--;
	}
	if (var2[i] == '\0' || var2[i] == '=')
		return (0);
	return (var1[i] - var2[i]);
}

int	get_var_name_len(char *var)
{
	int	i;

	i = 0;
	while (var[i] && var[i] != '=')
		i++;
	return (i);
}

char	*get_env_var(char *var_name, char **env)
{
	int		i;

	if (env == NULL || var_name == NULL)
		return (NULL);
	i = 0;
	while (env[i])
	{
		if (compare_env_var_names(var_name, env[i], get_var_name_len(var_name)) == SUCCESS)
			return (env[i]);
		i++;
	}
	return (NULL);
}

char	*get_env_var_value(char *var_name, char **env)
{
	char	*var_value;
	int		i;

	var_value = get_env_var(var_name, env);
	if (var_value == NULL || !is_var_value_present(var_value))
		return (NULL);
	i = 0;
	while (var_value[i] != '=')
		i++;
	return (&var_value[i + 1]);
}
