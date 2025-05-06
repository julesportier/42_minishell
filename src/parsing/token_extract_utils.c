/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token_extract_utils.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: juportie <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/28 16:14:05 by juportie          #+#    #+#             */
/*   Updated: 2025/05/06 15:25:00 by juportie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include "parsing.h"
#include "lexer.h"

void	extract_two_char(t_token *token, enum e_token_type type, int *pos)
{
	advance(2, pos);
	token->str = NULL;
	token->type = type;
}

void	extract_one_char(t_token *token, enum e_token_type type, int *pos)
{
	advance(1, pos);
	token->str = NULL;
	token->type = type;
}
