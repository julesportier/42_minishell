/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token_extract_heredoc.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ecasalin <ecasalin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/22 11:14:52 by juportie          #+#    #+#             */
/*   Updated: 2025/06/07 10:06:52 by ecasalin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>
#include <stdlib.h>
#include <readline/readline.h>
#include <readline/history.h>
#include "../../libft/src/libft.h"
#include "../minishell.h"
#include "../general_utils/utils.h"
#include "parsing.h"
#include "lexer.h"

static char	*join_heredoc_content(char *heredoc_content, char *new_content, t_error *error)
{
	heredoc_content = free_strjoin(heredoc_content, new_content, true, true);
	if (heredoc_content == NULL)
	{
		*error = critical;
		return (NULL);
	}
	heredoc_content = free_strjoin(heredoc_content, "\n", true, false);
	if (heredoc_content == NULL)
	{
		*error = critical;
		return (NULL);
	}
	return (heredoc_content);
}

static t_bool	is_heredoc_end(char *heredoc_line, char *delimiter)
{
	if (heredoc_line == NULL)
	{
		ft_putstr_fd("minishell: warning unexpected end-of-file in heredoc\n", 2);
		return (true);
	}
	else if (ft_isequalstr(delimiter, heredoc_line))
	{
		free(heredoc_line);
		return (true);
	}
	else
		return (false);
}

// static void	remove_last_newline(char *heredoc_content)
// {
// 	int		content_len;
//
// 	content_len = ft_strlen(heredoc_content);
// 	if (content_len > 0)
// 		heredoc_content[content_len - 1] = '\0';
// }

t_error	extract_heredoc(t_token *token, char *line, int *pos)
{
	char	*heredoc_line;
	char	*heredoc_content;
	t_error	error;

	error = success;
	if (extract_delimiter(token, line, pos, &error) != success)
		return (error);
	heredoc_content = NULL;
	while (1)
	{
		heredoc_line = readline("> ");
		if (is_heredoc_end(heredoc_line, token->str))
			break ;
		heredoc_content = join_heredoc_content(heredoc_content, heredoc_line, &error);
		if (error)
			return (error);
	}
	// remove_last_newline(heredoc_content);
	free(token->str);
	token->str = heredoc_content;
	return (error);
}
