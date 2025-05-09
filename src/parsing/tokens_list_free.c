/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokens_list_free.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: juportie <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/07 10:56:00 by juportie          #+#    #+#             */
/*   Updated: 2025/05/07 11:01:11 by juportie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parsing.h"

// The function to give as second argument to ft_dlstclear() or ft_dlstremove()
void	free_token_content(void *content)
{
	free(((t_token *)content)->str);
	free(content);
}
