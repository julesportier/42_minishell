/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signal_handlers.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ecasalin <ecasalin@42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/02 14:04:24 by ecasalin          #+#    #+#             */
/*   Updated: 2025/06/12 10:08:50 by ecasalin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <unistd.h>
#include <signal.h>
#include <readline/readline.h>
#include "../signals_utils/signals_utils.h"

void	sigint_input_handler(int sig)
{
	g_sig = sig;
	rl_done = 1;
	rl_on_new_line();
	rl_replace_line("", 0);
	rl_redisplay();
}

void	handler_set_g_sig(int sig)
{
	g_sig = sig;
	rl_done = 1;
}
