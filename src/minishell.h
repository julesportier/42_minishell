/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: juportie <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/22 16:31:37 by juportie          #+#    #+#             */
/*   Updated: 2025/04/22 17:51:15 by juportie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

typedef enum	e_bool
{
	false = 0,
	true = 1
}	t_bool;

typedef enum	e_error
{
	success = 0,
	critical,
	recoverable
}	t_error;

#endif
