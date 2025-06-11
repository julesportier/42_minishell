/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signal_handlers.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ecasalin <ecasalin@42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/02 14:04:24 by ecasalin          #+#    #+#             */
/*   Updated: 2025/06/11 21:24:20 by ecasalin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <unistd.h>
#include <signal.h>
#include <readline/readline.h>
#include "../signals_utils/signals_utils.h"

void	sigint_input_handler(int sig)
{
	(void)sig;
	rl_done = 1;
}

void	handler_set_g_sig(int sig)
{
	g_sig = sig;
	rl_done = 1;
}

void	sigint_heredoc_handler(int sig)
{
	g_sig = sig;
	rl_done = 1;
}
