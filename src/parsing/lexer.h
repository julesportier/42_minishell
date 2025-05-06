/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: juportie <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/25 09:47:43 by juportie          #+#    #+#             */
/*   Updated: 2025/05/06 15:24:47 by juportie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef LEXER_H
# define LEXER_H

# include "../minishell.h"
# include "parsing.h"

//typedef	enum	e_expansion
//{
//	normal,
//	double_quotes,
//	single_quotes,
//}	t_expansion;

t_bool	is_blank(char c);
t_bool	is_quote(char str);
t_bool	is_operator(char *str);
t_bool	is_expanding(char str);
void	advance(int i, int *pos);
t_bool	match(char *str, char c);
t_bool	skip_spaces(char *line, int *pos);
void	extract_two_char(t_token *token, enum e_token_type type, int *pos);
void	extract_one_char(t_token *token, enum e_token_type type, int *pos);
int	extract_quotes(t_token *token, char *line, int *pos);
int	extract_literal(t_token *token, char *line, int *pos);
int	extract_variable_identifier(t_token *token, char *line, int *pos);
void	extract_operator(t_token *token, char *line, int *pos);
int	extract_expanding(t_token *token, char *line, int *pos);

#endif
