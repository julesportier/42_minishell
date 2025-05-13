/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokens_list_free.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: juportie <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/07 10:56:00 by juportie          #+#    #+#             */
/*   Updated: 2025/05/13 17:22:17 by juportie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parsing.h"

// The function to give as second argument to ft_dlstclear() or ft_dlstremove()
void	free_token_content(void *content)
{
	free(((t_token *)content)->str);
	free(content);
}

void	free_toklist(t_dlst **toklist)
{
	ft_dlstclear(*toklist, free_token_content);
	*toklist = NULL;
}
