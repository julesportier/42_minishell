/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   input.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: juportie <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/21 11:16:48 by juportie          #+#    #+#             */
/*   Updated: 2025/05/21 15:34:19 by juportie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef INPUT_H
# define INPUT_H

# include "../minishell.h"

// input_signal_handling.c
int	init_sigaction(void);
// input.c
void	input_loop(t_shell_vars *vars, t_error *error);

#endif
