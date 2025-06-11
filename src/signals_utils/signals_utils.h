/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signals_utils.h                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ecasalin <ecasalin@42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/27 18:21:52 by ecasalin          #+#    #+#             */
/*   Updated: 2025/06/11 08:54:10 by juportie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <signal.h>

extern volatile __sig_atomic_t	g_sig;

/*SIGNAL HANDLERS*/
void	sigint_input_handler(int sig);
void	handler_set_g_sig(int sig);
void	sigint_heredoc_handler(int sig);

/*SIGINT INIT*/
int		init_sigint_exec_sigaction(void);
int		init_sigint_input_sigaction(void);
int		init_sigint_heredoc_sigaction(void);

/*SIGQUIT INIT*/
int		init_sigquit_exec_sigaction(void);
int		init_sigquit_input_sigaction(void);

/*SIGACTION INIT*/
int		init_input_sigaction(void);
int		init_exec_sigaction(void);
