/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signals.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ecasalin <ecasalin@42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/27 13:29:08 by ecasalin          #+#    #+#             */
/*   Updated: 2025/06/01 15:00:24 by ecasalin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

// #include <unistd.h>
// #include <signal.h>

// static void	child_sigint_handler(int sig)
// {
// 	wait(NULL);
// 	wait(NULL);
// 	exit(sig);
// }

// static void	sigint_handler(int sig)
// {
// 	(void)sig;
// 	printf("\n");
// 	rl_on_new_line();
// 	rl_replace_line("", 0);
// 	rl_redisplay();
// }

// int	init_child_sigint_sigaction(void)
// {
// 	struct sigaction	sigact;

// 	ft_bzero(&sigact, sizeof(struct sigaction));
// 	sigact.sa_flags = SA_RESTART;
// 	sigact.sa_sigaction = &child_sigint_handler;
// 	if (sigemptyset(&sigact->sa_mask) == -1)
// 		return (-1);
// 	if (sigaction(SIGINT, &sigact, NULL) == -1)
// 		return (-1);
// 	return (0);
// }

// int	init_sigint_sigaction(void)
// {
// 	struct sigaction	sigact;

// 	ft_bzero(&sigact, sizeof(struct sigaction));
// 	sigact.sa_flags = SA_RESTART;
// 	sigact.sa_sigaction = &sigint_handler;
// 	if (sigemptyset(&sigact->sa_mask) == -1)
// 		return (-1);
// 	if (sigaction(SIGINT, &sigact, NULL) == -1)
// 		return (-1);
// 	if (sigaction(SIGQUIT, &sigact, NULL) == -1)
// 		return (-1);
// 	return (0);
// }

// int	init_sigaction(void)
// {
// 	if (init_sigint_sigaction())
// 		return (-1);
// 	// if (init_sigquit_sigaction())
// 	// 	return (-1);
// 	return (0);
// }

// int	init_child_sigaction(void)
// {
// 	if (init_child_sigint_sigaction())
// 		return (-1);
// 	// if (init_child_sigquit_sigaction())
// 	// 	return (-1);
// 	return (0);
// }
