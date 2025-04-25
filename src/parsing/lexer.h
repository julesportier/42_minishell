/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: juportie <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/25 09:47:43 by juportie          #+#    #+#             */
/*   Updated: 2025/04/25 13:23:51 by juportie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef LEXER_H
# define LEXER_H

# include "../minishell.h"

//typedef	enum	e_expansion
//{
//	normal,
//	double_quotes,
//	single_quotes,
//}	t_expansion;

t_bool	is_separator(char c);
t_bool	is_meta(char *str);
void	advance(int i, int *pos);
t_bool	skip_spaces(char *line, int *pos);

#endif
