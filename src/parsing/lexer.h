/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: juportie <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/25 09:47:43 by juportie          #+#    #+#             */
/*   Updated: 2025/06/16 11:30:47 by juportie         ###   ########.fr       */
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

// char_identity_check.c
t_bool	is_blank(char c);
t_bool	is_quote(char str);
t_bool	is_operator(char *str);
t_bool	is_expanding(char str);
// lexer_utils.c
void	advance(int i, int *pos);
t_bool	match(char *str, char c);
t_bool	skip_spaces(char *line, int *pos);
t_token	*alloc_token(t_error *error);
// token_extract_utils.c
void	consume_two_char(t_token *token, enum e_token_type type, int *pos);
void	consume_one_char(t_token *token, enum e_token_type type, int *pos);
// token_extract.c
t_error	extract_quotes(t_token *token, char *line, int *pos);
t_error	extract_literal(t_token *token, char *line, int *pos);
t_error	extract_variable_identifier(t_token *token, char *line, int *pos);
t_error	extract_operator(t_token *token, char *line, int *pos);
t_error	extract_expanding(t_token *token, char *line, int *pos);
// token_extract_grouping.c
t_error	extract_grouping(t_token *token, char *line, int *pos);
// token_extract_heredoc.c
t_error	extract_heredoc(t_token **token, char *line, int *pos);
// token_extract_heredoc_delimiter.c
t_error	extract_delimiter(t_token *token, char *line, int *pos, t_error *error);

#endif
