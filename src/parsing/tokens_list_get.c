/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokens_list_get.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: juportie <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/06 10:59:37 by juportie          #+#    #+#             */
/*   Updated: 2025/05/06 11:51:44 by juportie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parsing.h"

// /!\ The three functions below must be called after checking
// that the tokens list is not NULL.
enum e_token_type	get_toklist_type(t_dlst *list)
{
	return (((t_token *)(list->content))->type);
}

char	*get_toklist_str(t_dlst *list)
{
	return (((t_token *)(list->content))->str);
}

t_bool	get_toklist_cat_prev(t_dlst *list)
{
	return (((t_token *)(list->content))->cat_prev);
}
