/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   input.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ecasalin <ecasalin@42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/21 11:16:48 by juportie          #+#    #+#             */
/*   Updated: 2025/06/01 16:20:38 by ecasalin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef INPUT_H
# define INPUT_H

# include "../minishell.h"

// input_signal_handling.c
int	init_input_sigaction(void);
int	init_exec_sigaction(void);
int	init_child_sigaction(void);
int	init_sigquit_input_sigaction(void);
int	init_sigquit_exec_sigaction(void);
// input.c
void	input_loop(t_shell_vars *vars, t_error *error);

#endif
