/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ecasalin <ecasalin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/25 15:41:16 by ecasalin          #+#    #+#             */
/*   Updated: 2025/06/18 14:37:31 by ecasalin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include <unistd.h>
#include "../../libft/src/libft.h"
#include "../error_handling/errors.h"
#include "../general_utils/utils.h"
#include "../shell_init/init.h"
#include "../cleaning_utils/cleaning.h"
#include "builtins.h"
#include "../minishell.h"

static int	is_valid_var_name(char *var)
{
	int	i;

	if (var == NULL || (!ft_isalpha(var[0]) && var[0] != '_'))
		return (print_var_name_error(var));
	i = 1;
	while (var[i] && var[i] != '=')
	{
		if (!ft_isalnum(var[i]) && var[i] != '_')
			return (print_var_name_error(var));
		i++;
	}
	return (SUCCESS);
}

char	**add_var_to_env(char *var, char **env)
{
	int		i;
	char	**new_env_array;

	i = count_array_len(env);
	new_env_array = malloc((i + 2) * sizeof(char *));
	if (new_env_array == NULL)
	{
		free_array(env);
		return (NULL);
	}
	i = -1;
	while (env[++i])
		new_env_array[i] = env[i];
	new_env_array[i] = ft_strdup_s(var);
	if (new_env_array[i] == NULL)
	{
		free(new_env_array);
		return (NULL);
	}
	free(env);
	new_env_array[i + 1] = NULL;
	return (new_env_array);
}

int	update_var(char *var, char *new_var_value, char **env)
{
	int	i;

	if (!is_var_value_present(new_var_value))
		return (SUCCESS);
	i = 0;
	while (env[i] != var)
		i++;
	free(env[i]);
	env[i] = ft_strdup_s(new_var_value);
	if (env[i] == NULL)
		return (CRIT_ERROR);
	return (SUCCESS);
}

static int	add_or_update_var(char *var, t_shell_vars *vars)
{
	int		return_value;
	char	*temp_var;
	char	**temp_env;

	return_value = is_valid_var_name(var);
	if (return_value == CRIT_ERROR)
		return (CRIT_ERROR);
	else if (return_value == SUCCESS)
	{
		temp_var = get_env_var(var, vars->env);
		if (temp_var == NULL)
		{
			temp_env = add_var_to_env(var, vars->env);
			if (temp_env == NULL)
				return (CRIT_ERROR);
			vars->env = temp_env;
		}
		else
			if (update_var(temp_var, var, vars->env) == CRIT_ERROR)
				return (CRIT_ERROR);
	}
	return (return_value);
}

int	ms_export(char **args, t_shell_vars *vars)
{
	int		i;
	int		return_value;
	int		is_error;

	is_error = 0;
	if (args[0] == NULL)
	{
		return_value = print_sorted_env(vars->env);
		if (return_value != SUCCESS)
			return (print_export_failure(return_value));
		return (SUCCESS);
	}
	i = 0;
	while (args[i])
	{
		return_value = add_or_update_var(args[i], vars);
		if (return_value == CRIT_ERROR)
			return (print_export_failure(return_value));
		else if (return_value == ERROR)
			is_error = ERROR;
		i++;
	}
	if (is_error)
		return (ERROR);
	return (SUCCESS);
}
