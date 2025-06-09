/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expansions_wildcard_append_junk.c                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: juportie <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/07 10:35:33 by juportie          #+#    #+#             */
/*   Updated: 2025/06/09 17:44:04 by juportie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../libft/src/libft.h"
#include "../error_handling/errors.h"
#include "../general_utils/utils.h"
#include "expansions.h"

static char	*set_prefix(char *wildcard_str, t_error *error)
{
	int		prefix_len;
	char	*prefix;

	prefix_len = get_junk_start_len(wildcard_str);
	if (prefix_len > 0)
	{
		prefix = ft_substr(wildcard_str, 0, prefix_len);
		if (!prefix)
			return (set_err_return_null(error, critical));
		return (prefix);
	}
	else
		return (NULL);
}

static char	*set_suffix(char *wildcard_str, t_error *error)
{
	char	*suffix;

	if (is_matching_directories(wildcard_str))
	{
		suffix = ft_strdup("/");
		if (!suffix)
			return (set_err_return_null(error, critical));
		return (suffix);
	}
	else
		return (NULL);
}

static t_error	set_prefix_and_suffix(
	char *wildcard_str,
	char **prefix,
	char **suffix,
	t_error *error)
{
	*prefix = set_prefix(wildcard_str, error);
	if (*error)
		return (*error);
	*suffix = set_suffix(wildcard_str, error);
	if (*error)
		free(prefix);
	return (*error);
}

static t_error	append_junk_to_item(
	t_dlst	*item,
	char	*prefix,
	char	*suffix,
	t_error *error)
{
	set_toklist_str(
		item,
		free_strjoin(prefix, get_toklist_str(item), false, true));
	// Interrupt free_strjoin if malloc fails (return (NULL) and don't free nothing) ??
	if (!get_toklist_str(item))
		set_err_return_err_enun(error, critical);
	set_toklist_str(
		item,
		free_strjoin(get_toklist_str(item), suffix, true, false));
	if (!get_toklist_str(item))
		set_err_return_err_enun(error, critical);
	return (*error);
}

t_error	append_junk_to_cwd_list(
	t_dlst	*cwd_content_list,
	char	*wildcard_str,
	t_error	*error)
{
	char	*prefix;
	char	*suffix;

	if (set_prefix_and_suffix(wildcard_str, &prefix, &suffix, error) != success)
		return (*error);
	while (cwd_content_list)
	{
		if (append_junk_to_item(
				cwd_content_list, prefix, suffix, error) != success)
			break ;
		cwd_content_list = cwd_content_list->next;
	}
	free(prefix);
	free(suffix);
	return (*error);
}
