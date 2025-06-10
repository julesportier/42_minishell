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

#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <readline/readline.h>
#include <readline/history.h>
#include <unistd.h>
#include <sys/ioctl.h>
// #include <stropts.h>
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

static void	sigint_heredoc_handler(int sig)
{
	// char	*tty;
	// int	tty_fd;
	//
	// (void)sig;
	// tty = ttyname(STDIN_FILENO);
	// tty_fd = open(tty, O_WRONLY);
	// // dprintf(tty_fd, "ttyname: %s\n", tty);
	// // write(tty_fd, (char []){(char)EOF}, 1);
	// // write(tty_fd, "\0", 1);
	// // ioctl(tty_fd, _IOW('a', 'a', char*), "\n");
	// // write(1, (char []){(char)-1}, 1);
	// // rl_on_new_line();
	// char	eof = -1;
	//
	// rl_replace_line(&eof, 0);
	// rl_redisplay();
	// // write(tty_fd, "\n", 2);
	// close(tty_fd);
	// // write(1, "\x04", 1);
	// // write(1, "\n", 1);
	g_sig = sig;
	close(STDIN_FILENO);
	// write(1, "in handler\n", 10);
}

static void	init_sigint_heredoc_sigaction(void)
{
	struct sigaction	sigact;

	ft_bzero(&sigact, sizeof(struct sigaction));
	sigact.sa_flags = SA_RESTART;
	sigact.sa_handler = sigint_heredoc_handler;
	sigemptyset(&sigact.sa_mask);
	sigaction(SIGINT, &sigact, NULL);
	// sigaction(SIGINT, NULL, &sigact);
}

t_error	extract_heredoc(t_token *token, char *line, int *pos)
{
	char	*heredoc_line;
	char	*heredoc_content;
	int		fd;
	t_error	error;

	init_sigint_heredoc_sigaction();
	error = success;
	if (extract_delimiter(token, line, pos, &error) != success)
		return (error);
	heredoc_content = NULL;
	while (1)
	{
		fd = dup(STDIN_FILENO);
		heredoc_line = readline("> ");
		// printf("heredoc_line content: '%s'\n", heredoc_line);
		write(1, "after readline\n", 15);
		printf("EOF == %d\n", EOF);
		// printf("g_sig heredoc == '%d'\n", g_sig);
		if (g_sig)
		{
			dup2(fd, STDIN_FILENO);
			break ;
		}
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
