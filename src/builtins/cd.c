/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ecasalin <ecasalin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/24 07:21:49 by ecasalin          #+#    #+#             */
/*   Updated: 2025/05/05 13:32:26 by ecasalin         ###   ########.fr       */
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

static char	*strjoin_free_s2(char *s1, char *s2)
{
	char *joined_str;

	joined_str = ft_strjoin(s1, s2);
	free(s2);
	return (joined_str);
}

char	*concat_possible_paths(char *path, char **env)
{
	char	*cdpath;
	char	*temp_path;
	int		temp;
	
	cdpath = get_env_var_value("CDPATH", env);
	if (cdpath == NULL)
		return (ft_strjoin("./", path)); //check fail
	if (cdpath[0] == ':')
	{
		temp_path = free_strjoin(path, )
		temp = access()
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
		path = get_env_var_value("OLDPWD", vars->env); //faut print le path si ca marche
	if (path == NULL)
		return (ERROR);
	if (ft_strncmp(args[0], "./", 2) != SUCCESS &&
		ft_strncmp(args[0], "../", 3) != SUCCESS &&
		ft_strncmp(args[0], "/", 1) != SUCCESS)
	{
			path = concat_possible_paths(args[0], vars->env);
	
	//si pas de . .. ou / au debut, on concat le premier path present dans CDPATH (rajouter un slash si pas present dans le CDPATH) avec path.
	//si c'est un path valide on garde lui, sinon on concat avec ./
	//si c'est valide on le garde, sinon on concat avec tous les paths restants dans CDPATH et on garder le premier valide.
	//si aucun n'est valide on print erreur minishell: cd: "path": No such file or directotry
	//si on trouve un path grace a CDPATH on le print
	if (ft_strrchr(path, '/'))

}
