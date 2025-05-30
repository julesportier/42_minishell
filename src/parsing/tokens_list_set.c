/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokens_list_set.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: juportie <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/06 10:59:37 by juportie          #+#    #+#             */
/*   Updated: 2025/05/26 14:40:29 by juportie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parsing.h"

// /!\ The three functions below must be called after checking
// that the tokens list is not NULL.
void	set_toklist_type(t_dlst *list, enum e_token_type type)
{
	((t_token *)(list->content))->type = type;
}

void	set_toklist_str(t_dlst *list, char *str)
{
	((t_token *)(list->content))->str = str;
}

void	set_toklist_cat_prev(t_dlst *list, t_bool cat_prev)
{
	((t_token *)(list->content))->cat_prev = cat_prev;
}
