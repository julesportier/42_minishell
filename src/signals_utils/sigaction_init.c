/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sigaction_init.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ecasalin <ecasalin@42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/27 13:29:08 by ecasalin          #+#    #+#             */
/*   Updated: 2025/06/09 09:46:45 by ecasalin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../signals_utils/signals_utils.h"

int	init_input_sigaction(void)
{
	if (init_sigint_input_sigaction())
		return (-1);
	if (init_sigquit_input_sigaction())
		return (-1);
	return (0);
}

int	init_exec_sigaction(void)
{
	if (init_sigint_exec_sigaction())
		return (-1);
	if (init_sigquit_exec_sigaction())
		return (-1);
	return (0);
}
