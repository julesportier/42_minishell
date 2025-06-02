/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sigquit_init.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ecasalin <ecasalin@42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/02 14:13:30 by ecasalin          #+#    #+#             */
/*   Updated: 2025/06/02 15:00:31 by ecasalin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <unistd.h>
#include <signal.h>
#include "../../libft/src/libft.h"
#include "../signals_utils/signals_utils.h"

int init_sigquit_exec_sigaction(void)
{
	struct sigaction sigact;

	ft_bzero(&sigact, sizeof(struct sigaction));
	sigact.sa_flags = SA_RESTART;
	sigact.sa_handler = handler_set_g_sig;
	if (sigemptyset(&sigact.sa_mask) == -1)
		return (-1);
	if (sigaction(SIGQUIT, &sigact, NULL) == -1)
		return (-1);
	return (0);
}

int init_sigquit_input_sigaction(void)
{
	struct sigaction sigact;

	ft_bzero(&sigact, sizeof(struct sigaction));
	sigact.sa_flags = SA_RESTART;
	sigact.sa_handler = SIG_IGN;
	if (sigemptyset(&sigact.sa_mask) == -1)
		return (-1);
	if (sigaction(SIGQUIT, &sigact, NULL) == -1)
		return (-1);
	return (0);
}
