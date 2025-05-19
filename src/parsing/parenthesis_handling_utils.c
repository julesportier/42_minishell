/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parenthesis_handling_utils.c                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: juportie <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/14 10:22:13 by juportie          #+#    #+#             */
/*   Updated: 2025/05/14 10:31:26 by juportie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parsing.h"

int	update_parenthesis_nesting_level(t_dlst *token, int nesting_level)
{
	if (get_toklist_type(token) == right_parenthesis)
		return (--nesting_level);
	else if (get_toklist_type(token) == left_parenthesis)
		return (++nesting_level);
	else
		return (nesting_level);
}

t_error	check_nesting_level(int nesting_level, t_dlst *toklist)
{
	if (nesting_level < 0 || (toklist->next == NULL && nesting_level != 0))
		return (recoverable);
	else
		return (success);
}
