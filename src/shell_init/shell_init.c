/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   shell_init.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ecasalin <ecasalin@42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/24 08:01:52 by ecasalin          #+#    #+#             */
/*   Updated: 2025/06/09 09:53:42 by ecasalin         ###   ########.fr       */
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
		ft_putstr_fd("minishell: current working directory is unlinked: "
			"setting internal backup cwd to /\n", 2);
		errno = 0;
	}
	return (cwd);
}

int	init_shell_vars(t_shell_vars *vars, char **envp, t_error *error)
{
	vars->prompt = NULL;
	vars->last_cmd_ext_code = SUCCESS;
	vars->env = init_env_array(envp, error);
	if (*error == critical)
		return (CRIT_ERROR);
	vars->cwd_backup = init_cwd_backup();
	if (vars->cwd_backup == NULL)
	{
		free_array(vars->env);
		return (CRIT_ERROR);
	}
	return (SUCCESS);
}
