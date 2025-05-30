/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expansions_wildcard.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: juportie <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/26 10:25:15 by juportie          #+#    #+#             */
/*   Updated: 2025/05/30 16:23:40 by juportie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <dirent.h>
#include "expansions.h"

static t_dlst	*set_wildcard_to_literal(t_dlst *token, t_error *error)
{
	if (replace_token_content((t_token *)(token->content), literal, "*", error)
			!= success)
		return (NULL);
	return (token);
}

t_dlst	*expand_wildcard(t_dlst *token, t_dlst **toklist, t_error *error)
{
	char			cwd[PATH_MAX];
	DIR				*dir_stream;
	struct dirent	*dir_stat;

	if (!getcwd(cwd, PATH_MAX))
		return (set_wildcard_to_literal(token, error));
	dir_stream = opendir(cwd);
	// IF DIRECTORY NOT EXIST OR USER DON'T HAVE PERMISSIONS BASH JUST RETURNS THE LITERAL '*'
	// CHECK IF ON OTHER ERRORS IT PRINT AN ERROR MESSGAGE END WHAT IT DOES
	if (!dir_stream)
		return (set_wildcard_to_literal(token, error));
	while (1)
	{
		dir_stat = readdir(dir_stream);
		if (!dir_stat)
			break ;
		//printf("readdir-> `%s'\n", dir_stat->d_name);
		if (dir_stat->d_name[0] != '.')
		{
			if (place_expansion_result(&token, wildcard, dir_stat->d_name, error) != success)
				return (NULL);
		}
	}
	closedir(dir_stream);
	if (get_toklist_type(token) == wildcard)
		return (set_wildcard_to_literal(token, error));
	return (token);
	(void)toklist;
}
