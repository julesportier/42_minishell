/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   input.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ecasalin <ecasalin@42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/21 11:16:48 by juportie          #+#    #+#             */
/*   Updated: 2025/06/02 15:15:36 by ecasalin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef INPUT_H
# define INPUT_H

# include "../minishell.h"

/*INPUT LOOP*/
void	input_loop(t_shell_vars *vars, t_error *error);
char	*set_readline_and_history(char *prompt, t_error *error);
void	create_prompt(t_shell_vars *vars, char *personalized_prompt, t_error *error);

#endif
