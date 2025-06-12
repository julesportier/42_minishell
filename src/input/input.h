/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   input.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ecasalin <ecasalin@42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/21 11:16:48 by juportie          #+#    #+#             */
/*   Updated: 2025/06/12 10:14:34 by ecasalin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef INPUT_H
# define INPUT_H

# ifndef PROMPT_SUCCESS
#  define PROMPT_SUCCESS " \033[1;32m❯\033[0m "
# endif
# ifndef PROMPT_FAILURE
#  define PROMPT_FAILURE " \033[1;31m❯\033[0m "
# endif

# include "../minishell.h"

/*INPUT LOOP*/
void	input_loop(t_shell_vars *vars, t_error *error);
char	*set_readline_and_history(char *prompt, t_error *error,
			t_shell_vars *vars);
void	create_prompt(t_shell_vars *vars, t_error *error);
int		refresh_rl_vars(void);

#endif
