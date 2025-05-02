/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ecasalin <ecasalin@42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/24 07:21:49 by ecasalin          #+#    #+#             */
/*   Updated: 2025/05/02 16:16:33 by ecasalin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <unistd.h>
#include "../../libft/src/libft.h"
#include "../minishell.h"
#include "../general_utils/utils.h"
#include "../error_handling/errors.h"

char	*set_path_to_home(char **env)
{
	char	*path;

	path = get_env_var_value("HOME", env);
	if (path == NULL)
		ft_putstr_fd("minishell: cd: HOME not set\n", 2);
	return (path);
}

char	*set_oldpwd_to_home(char **env)
{
	char	*path;

	path = get_env_var_value("OLDPWD", env);
	if (path == NULL)
		ft_putstr_fd("minishell: cd: OLDPWD not set\n", 2);
	return (path);
}

int	ms_cd(char **args, t_shell_vars *vars)
{
	int		args_number;
	char	*path;

	args_number = count_array_len(args);
	if (args_number > 1)
		return (return_error("minishell: cd: to many arguments", ERROR));
	else if (args_number == 0)
		path = set_path_to_home(vars->env);
	else if (ft_strncmp(args[0], "-", 2) == SUCCESS)
		path = get_env_var_value("OLDPWD", vars->env);
	if (path == NULL)
		return (ERROR);
	//si pas de . .. ou / au debut, on concat le premier path present dans CDPATH (rajouter un slash si pas present dans le CDPATH) avec path.
	//si c'est un path valide on garde lui, sinon on concat avec ./
	//si c'est valide on le garde, sinon on concat avec tous les paths restants dans CDPATH et on garder le premier valide.
	//si aucun n'est valide on print erreur minishell: cd: "path": No such file or directotry
	//si on trouve un path grace a CDPATH on le print
	if (ft_strrchr(path, '/'))

}
