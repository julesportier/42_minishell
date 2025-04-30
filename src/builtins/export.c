/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ecasalin <ecasalin@42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/25 15:41:16 by kura              #+#    #+#             */
/*   Updated: 2025/04/30 14:31:12 by ecasalin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <unistd.h>
#include "../../libft/src/libft.h"
#include "../error_handling/errors.h"
#include "../general_utils/utils.h"
#include "../shell_init/init.h"
#include "../cleaning_utils/cleaning.h"
#include "../minishell.h"

// /*FAUT PASSER LE = AUSSI*/
char	*get_env_var(char *var_name, char **env)
{
	int		i;

	if (env == NULL || var_name == NULL)
		return (NULL);
	i = 0;
	while (env[i])
	{
		if (ft_strncmp(env[i], var_name, ft_strlen(var_name)) == SUCCESS)
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
	if (var_value == NULL)
		return (NULL);
	i = 0;
	while (var_value[i] != '=')
		i++;
	return (&var_value[i + 1]);
}

// /*POUR RAJOUTER DU PIQUANT, SI VAR = NULL CA SEGFAULT*/
static char	**add_var_to_env(char **env, char *var)
{
	int		i;
	char	**new_env_array;

	i = count_array_len(env);
	new_env_array = malloc((i + 2) * sizeof(char *));
	if (new_env_array == NULL)
		return (NULL);
	i = 0;
	while (env[i])
	{
		new_env_array[i] = env[i];
		i++;
	}
	new_env_array[i] = ft_strdup(var);
	if (new_env_array[i] == NULL)
	{
		free(new_env_array);
		return (NULL);
	}
	new_env_array[i + 1] = NULL;
	free(env);
	return (new_env_array);
}

static char	*find_biggest_env_var(char **env)
{
	int		i;
	char	*biggest_var;

	if (env == NULL)
		return (NULL);
	i = 0;
	biggest_var = env[0];
	while (env[i])
	{
		if (ft_strncmp(env[i], biggest_var, ft_strlen(biggest_var)) > 0)
			biggest_var = env[i];
		i++;
	}
	return (biggest_var);
}

static int	print_formatted_env_var(char *var)
{
	char	*var_to_print;
	int		i;
	int		temp;

	i = 0;
	var_to_print = ft_calloc(ft_strlen(var) + 4, 1);
	if (var_to_print == NULL)
		return (CRIT_ERROR);
	while (var[i] != '=')
		i++;
	ft_memcpy(var_to_print, var, i + 1);
	var_to_print = free_strjoin(var_to_print, "\"");
	if (var_to_print == NULL)
		return (CRIT_ERROR);
	var_to_print = free_strjoin(var_to_print, &var[i + 1]);
	if (var_to_print == NULL)
		return (CRIT_ERROR);
	var_to_print = free_strjoin(var_to_print, "\"\n");
	if (var_to_print == NULL)
		return (CRIT_ERROR);
	temp = write(1, var_to_print, ft_strlen(var_to_print));
	free(var_to_print);
	if (temp == FAILURE)
		return (ERROR);
	return (SUCCESS);
}

static int	print_sorted_env(char **env)
{
	char	*smaller_var;
	char	*var_to_print;
	int		i;
	int		y;
	int		error;

	y = count_array_len(env);
	smaller_var = "\1";
	while (y > 0)
	{
		var_to_print = find_biggest_env_var(env);
		i = 0;
		while (env[i])
		{
			if (ft_strncmp(env[i], smaller_var, ft_strlen(smaller_var)) > 0 && ft_strncmp(env[i], var_to_print, ft_strlen(var_to_print)) < 0)
				var_to_print = env[i];
			i++;
		}
		y--;
		error = print_formatted_env_var(var_to_print);
		if (error)
			return (error);
		smaller_var = var_to_print;
	}
	return (SUCCESS);
}

int	ms_export(char **args, t_shell_vars *vars)
{
	int	i;
	int	temp;

	if (args[0] == NULL)
	{
		temp = print_sorted_env(vars->env);
		if (temp == CRIT_ERROR)
			return (return_perror("bash: export critical error", CRIT_ERROR));
		if (temp == ERROR)
			return (return_perror("bash export", ERROR));
		return (SUCCESS);
	}
	i = 0;
	while (args[i])
	{
		vars->env = add_var_to_env(vars->env, args[i]);
		if (vars->env == NULL)
			return (return_perror("bash: export critical error", CRIT_ERROR));
		i++;
	}
	return (SUCCESS);
}

int	main(int ac, char *av[], char *envp[])
{
	t_shell_vars	vars;
	t_error	error;

	vars.env = init_env_array(envp, &error);
	ms_export(&av[2], &vars);
	if (ac >= 3)
		print_sorted_env(vars.env);
	free_array(vars.env);
}
