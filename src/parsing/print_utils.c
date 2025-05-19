/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   print_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: juportie <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/13 11:09:35 by juportie          #+#    #+#             */
/*   Updated: 2025/05/13 11:11:02 by juportie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>
#include "../minishell.h"

void	print_indent(int indent_level)
{
	while (indent_level--)
		printf("   ");
}

const char	*bool_to_str(t_bool bool)
{
	if (bool == 0)
		return ("false");
	else
		return ("true");
}
