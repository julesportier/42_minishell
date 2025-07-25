/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc_to_file.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: juportie <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/26 10:25:15 by juportie          #+#    #+#             */
/*   Updated: 2025/05/30 17:00:55 by juportie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <fcntl.h>
#include "../../libft/src/libft.h"
#include "../minishell.h"
#include "../error_handling/errors.h"
#include "../parsing/parsing.h"

static t_error	get_random_uuid(char *str, t_error *error)
{
	int		fd;

	fd = open("/proc/sys/kernel/random/uuid", O_RDONLY);
	if (fd == -1)
		return (return_perror_set_err(
				"minishell: get_random_uuid", error, recoverable));
	str[36] = '\0';
	if (read(fd, str, 36) == -1)
	{
		return_perror_set_err(
			"minishell: get_random_uuid", error, recoverable);
	}
	close(fd);
	return (*error);
}

static char	*get_random_filename(t_error *error)
{
	char	uuid[37];
	char	*filename;

	if (get_random_uuid(uuid, error) != success)
		return (NULL);
	filename = ft_strjoin("/tmp/", uuid);
	if (!filename)
		return (null_perror_alloc(critical, error));
	return (filename);
}

static t_error	write_to_file(t_dlst *token, char *filename, t_error *error)
{
	int		fd;
	char	*heredoc_content;

	fd = open(filename, O_WRONLY | O_CREAT | O_EXCL, S_IRUSR | S_IWUSR);
	if (fd == -1)
		return (return_perror_set_err(
				"minishell: write_to_file", error, recoverable));
	heredoc_content = get_toklist_str(token);
	if (heredoc_content == NULL)
	{
		close(fd);
		return (success);
	}
	if (write(fd, heredoc_content, ft_strlen(heredoc_content)) == -1)
	{
		close(fd);
		return (return_perror_set_err(
				"minishell: write_to_file", error, recoverable));
	}
	close(fd);
	return (*error);
}

static t_error	move_heredoc_content_to_file(t_dlst *token, t_error *error)
{
	char	*filename;

	filename = get_random_filename(error);
	if (*error)
		return (*error);
	write_to_file(token, filename, error);
	if (*error)
	{
		free(filename);
		return (*error);
	}
	if (get_toklist_str(token))
		free(get_toklist_str(token));
	set_toklist_str(token, filename);
	return (success);
}

t_error	populate_heredocs_files(t_dlst *toklist, t_error *error)
{
	while (toklist)
	{
		if (get_toklist_type(toklist) == heredoc)
		{
			toklist = toklist->next;
			if (move_heredoc_content_to_file(toklist, error) != success)
				return (*error);
		}
		toklist = toklist->next;
	}
	return (success);
}
