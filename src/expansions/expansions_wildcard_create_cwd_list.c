/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expansions_wildcard_create_cwd_list.c              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: juportie <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/26 10:25:15 by juportie          #+#    #+#             */
/*   Updated: 2025/06/09 17:41:47 by juportie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <dirent.h>
#include <stdio.h>
#include <errno.h>
#include "../../libft/src/libft.h"
#include "../error_handling/errors.h"
#include "../parsing/parsing.h"
#include "../parsing/lexer.h"
#include "expansions.h"

static t_error	append_entry_to_list(
	t_dlst **cwd_content,
	char *entry,
	t_error *error)
{
	t_token	*token;

	token = alloc_token(error);
	if (*error)
		return (*error);
	token->str = ft_strdup(entry);
	if (!token->str)
	{
		free(token);
		return (set_err_return_err_enun(error, critical));
	}
	token->type = literal;
	append_token_to_list(cwd_content, token, error);
	if (*error)
	{
		free_token_content(token);
		free(token);
	}
	return (*error);
}

static t_dlst	*get_cwd_content_list(
	DIR *dir_stream,
	t_bool match_directories_only,
	t_error *error)
{
	t_dlst			*cwd_content;
	struct dirent	*dir_stat;

	cwd_content = NULL;
	while (1)
	{
		errno = 0;
		dir_stat = readdir(dir_stream);
		if (errno)
		{
			perror("minishell: readdir");
			*error = recoverable;
		}
		if (!dir_stat)
			break ;
		if (match_directories_only && dir_stat->d_type == DT_DIR)
			append_entry_to_list(&cwd_content, dir_stat->d_name, error);
		else if (!match_directories_only)
			append_entry_to_list(&cwd_content, dir_stat->d_name, error);
		if (*error)
			break ;
	}
	if (*error)
		free_toklist(&cwd_content);
	return (cwd_content);
}

static t_dlst	*remove_token_if(
	t_bool test(t_dlst *token),
	t_dlst *cwd_content_list)
{
	t_dlst	*head;
	t_dlst	*next;

	head = cwd_content_list;
	while (cwd_content_list)
	{
		next = cwd_content_list->next;
		if (test(cwd_content_list))
		{
			if (cwd_content_list->prev == NULL)
				head = head->next;
			ft_dlstremove(cwd_content_list, free_token_content, free);
		}
		cwd_content_list = next;
	}
	return (head);
}

t_dlst	*create_filtered_cwd_content_list(t_dlst *token, t_error *error)
{
	char	cwd[PATH_MAX];
	DIR		*dir_stream;
	t_dlst	*cwd_content_list;

	if (is_out_of_cwd(get_toklist_str(token)))
		return (NULL);
	if (!getcwd(cwd, PATH_MAX))
		return (NULL);
	dir_stream = opendir(cwd);
	if (!dir_stream)
		return (NULL);
	cwd_content_list = get_cwd_content_list(
			dir_stream, is_matching_directories(get_toklist_str(token)), error);
	closedir(dir_stream);
	if (!cwd_content_list)
		return (NULL);
	if (is_matching_hidden_files(get_toklist_str(token)))
		cwd_content_list = remove_token_if(
				is_non_hidden_file, cwd_content_list);
	else
		cwd_content_list = remove_token_if(is_hidden_file, cwd_content_list);
	return (cwd_content_list);
}
