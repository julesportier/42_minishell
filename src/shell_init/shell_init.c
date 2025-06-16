/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   shell_init.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ecasalin <ecasalin@42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/24 08:01:52 by ecasalin          #+#    #+#             */
/*   Updated: 2025/06/16 10:21:09 by juportie         ###   ########.fr       */
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
	int		env_array_len;

	*error = success;
	if (env == NULL)
		return (NULL);
	env_array_len = count_array_len(env);
	env_array = malloc((env_array_len + 1) * sizeof(char *));
	if (env_array == NULL)
		return (null_print_alloc_err(critical, error));
	env_array[env_array_len] = NULL;
	i = 0;
	while (i < env_array_len)
	{
		env_array[i] = ft_strdup_s(env[i]);
		if (env_array[i] == NULL)
		{
			free_array(env_array);
			return (null_print_alloc_err(critical, error));
		}
		i++;
	}
	return (env_array);
}

char	*init_cwd_backup(void)
{
	char		*cwd;

	errno = SUCCESS;
	cwd = getcwd(NULL, 0);
	if (errno == ENOMEM)
		return (return_perror_set_err_null("minishell: getcwd", NULL, critical));
	if (cwd == NULL)
	{
		cwd = ft_strdup_s("/");
		if (cwd == NULL)
			return (null_print_alloc_err(critical, NULL));
		ft_putstr_fd("minishell: current working directory is unlinked: "
			"setting internal backup cwd to /\n", 2);
		errno = SUCCESS;
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
