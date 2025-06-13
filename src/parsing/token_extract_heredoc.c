/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token_extract_heredoc.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ecasalin <ecasalin@42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/22 11:14:52 by juportie          #+#    #+#             */
/*   Updated: 2025/06/12 10:24:24 by ecasalin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <readline/readline.h>
#include <readline/history.h>
#include <unistd.h>
#include "../../libft/src/libft.h"
#include "../minishell.h"
#include "../general_utils/utils.h"
#include "../signals_utils/signals_utils.h"
#include "parsing.h"
#include "lexer.h"

static char	*join_heredoc_content(char *heredoc_content, char *new_content, t_error *error)
{
	heredoc_content = free_strjoin(heredoc_content, new_content, true, true);
	if (heredoc_content == NULL)
		return (null_perror_alloc(critical, error));
	heredoc_content = free_strjoin(heredoc_content, "\n", true, false);
	if (heredoc_content == NULL)
		return (null_perror_alloc(critical, error));
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

static char	*heredoc_input_loop(t_token *token, t_error *error)
{
	char	*heredoc_line;
	char	*heredoc_content;

	heredoc_content = NULL;
	while (1)
	{
		heredoc_line = readline("> ");
		if (g_sig)
		{
			free(heredoc_line);
			free(heredoc_content);
			return (NULL);
		}
		if (is_heredoc_end(heredoc_line, token->str))
			break ;
		heredoc_content = join_heredoc_content(heredoc_content, heredoc_line, error);
		if (*error)
			return (NULL);
	}
	return (heredoc_content);
}

t_error	extract_heredoc(t_token *token, char *line, int *pos)
{
	char	*heredoc_content;
	t_error	error;

	init_sigint_exec_sigaction();
	error = success;
	if (extract_delimiter(token, line, pos, &error) != success)
		return (error);
	heredoc_content = heredoc_input_loop(token, &error);
	init_sigint_input_sigaction();
	free(token->str);
	if (g_sig || error)
		free(token);
	else
		token->str = heredoc_content;
	return (error);
}
