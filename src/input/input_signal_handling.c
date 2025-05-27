/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   input_signal_handling.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ecasalin <ecasalin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/21 11:14:31 by juportie          #+#    #+#             */
/*   Updated: 2025/05/27 14:25:04 by ecasalin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include <signal.h>
#include <readline/readline.h>
#include <readline/history.h>
#include "../../libft/src/libft.h"
#include <sys/wait.h>

// static int	init_mask(struct sigaction *sigact)
// {
// 	if (sigemptyset(&sigact->sa_mask) == -1)
// 		return (-1);
// 	if (sigaddset(&sigact->sa_mask, SIGINT) == -1)
// 		return (-1);
// 	if (sigaddset(&sigact->sa_mask, SIGQUIT) == -1)
// 		return (-1);
// 	return (0);
// }

// static void	sigint_handler(int sig, siginfo_t *info, void *context)
// {
// 	(void)info;
// 	(void)context;
// 	(void)sig;
// 	printf("\n");
// 	rl_on_new_line();
// 	rl_replace_line("", 0);
// 	rl_redisplay();
// }

// static int	init_sigint_sigaction(void)
// {
// 	struct sigaction	sigact;

// 	ft_bzero(&sigact, sizeof(struct sigaction));
// 	sigact.sa_flags = SA_RESTART | SA_SIGINFO;
// 	sigact.sa_sigaction = &sigint_handler;
// 	if (init_mask(&sigact) == -1)
// 		return (-1);
// 	// SIGINT == <C-c>
// 	if (sigaction(SIGINT, &sigact, NULL) == -1)
// 		return (-1);
// 	return (0);
// }

// static int	init_sigquit_sigaction(void)
// {
// 	struct sigaction	sigact;

// 	ft_bzero(&sigact, sizeof(struct sigaction));
// 	sigact.sa_flags = SA_RESTART;
// 	sigact.sa_handler = SIG_IGN;
// 	if (init_mask(&sigact) == -1)
// 		return (-1);
// 	// SIGQUIT == <C-\>
// 	if (sigaction(SIGQUIT, &sigact, NULL) == -1)
// 		return (-1);
// 	return (0);
// }

// int	init_sigaction(void)
// {
// 	if (init_sigint_sigaction())
// 		return (-1);
// 	if (init_sigquit_sigaction())
// 		return (-1);
// 	return (0);
// }

static void	child_sigint_handler(int sig)
{
	printf("i exit with %d\n", sig);
	exit(sig);
}

static void	sigint_handler(int sig)
{
	(void)sig;
	printf("i am the father\n");
	printf("\n");
	rl_on_new_line();
	rl_replace_line("", 0);
	rl_redisplay();
}

int	init_child_sigint_sigaction(void)
{
	struct sigaction	sigact;
	
	ft_bzero(&sigact, sizeof(struct sigaction));
	sigact.sa_flags = SA_RESTART;
	sigact.sa_handler = child_sigint_handler;
	if (sigemptyset(&sigact.sa_mask) == -1)
		return (-1);
	if (sigaction(SIGINT, &sigact, NULL) == -1)
		return (-1);
	return (0);
}

int	init_sigint_sigaction(void)
{
	struct sigaction	sigact;
	
	ft_bzero(&sigact, sizeof(struct sigaction));
	sigact.sa_flags = SA_RESTART;
	sigact.sa_handler = sigint_handler;
	if (sigemptyset(&sigact.sa_mask) == -1)
		return (-1);
	if (sigaction(SIGINT, &sigact, NULL) == -1)
		return (-1);
	return (0);
}

int	init_sigaction(void)
{
	if (init_sigint_sigaction())
		return (-1);
	// if (init_sigquit_sigaction())
	// 	return (-1);
	return (0);
}

int	init_child_sigaction(void)
{
	if (init_child_sigint_sigaction())
		return (-1);
	// if (init_child_sigquit_sigaction())
	// 	return (-1);
	return (0);
}