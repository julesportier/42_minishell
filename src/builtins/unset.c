/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   unset.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ecasalin <ecasalin@42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/16 07:54:32 by ecasalin          #+#    #+#             */
/*   Updated: 2025/06/09 09:11:29 by ecasalin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <unistd.h>
#include "../minishell.h"
#include "../../libft/src/libft.h"
#include "../general_utils/utils.h"
#include "builtins.h"

static int	get_env_var_index(char *var_name, char **env, int env_len)
{
	int	i;

	i = 0;
	if (get_last_char(var_name) == '=')
		return (-1);
	while (i < env_len)
	{
		if (env[i] != NULL)
			if (compare_env_var_names(var_name, env[i],
					get_var_name_len(var_name)) == SUCCESS)
				return (i);
		i++;
	}
	return (-1);
}

static int	find_and_remove_var(char *var, t_shell_vars *vars, int env_len)
{
	int	i;

	i = get_env_var_index(var, vars->env, env_len);
	if (i == -1)
		return (0);
	free(vars->env[i]);
	vars->env[i] = NULL;
	return (1);
}

static char	**update_env(t_shell_vars *vars, int old_env_len, int removed_vars)
{
	int		i;
	int		y;
	char	**new_env;

	i = 0;
	y = 0;
	new_env = ft_calloc(old_env_len - removed_vars + 1, sizeof(char *));
	if (new_env == NULL)
		return (NULL);
	while (old_env_len >= 0)
	{
		if (vars->env[i] != NULL)
		{
			new_env[y] = vars->env[i];
			y++;
		}
		i++;
		old_env_len--;
	}
	free(vars->env);
	return (new_env);
}

int	ms_unset(char **args, t_shell_vars *vars)
{
	int	removed_vars;
	int	env_len;
	int	i;

	i = 0;
	if (args[0] == NULL || vars->env == NULL)
		return (SUCCESS);
	removed_vars = 0;
	env_len = count_array_len(vars->env);
	while (args[i])
	{
		removed_vars += find_and_remove_var(args[i], vars, env_len);
		i++;
	}
	vars->env = update_env(vars, env_len, removed_vars);
	if (vars->env == NULL)
		return (CRIT_ERROR);
	return (SUCCESS);
}
