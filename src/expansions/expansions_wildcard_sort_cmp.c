/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expansions_wildcard_sort_cmp.c                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: juportie <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/05 14:17:02 by juportie          #+#    #+#             */
/*   Updated: 2025/06/07 10:24:41 by juportie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../libft/src/libft.h"
#include "../parsing/parsing.h"

static int	to_lower(char c)
{
	if (c >= 'A' && c <= 'Z')
		return (c + 32);
	else
		return (c);
}

static int	ft_strncase(const char *s1, const char *s2, size_t n)
{
	size_t			i;
	unsigned char	*us1;
	unsigned char	*us2;

	i = 0;
	us1 = (unsigned char *)s1;
	us2 = (unsigned char *)s2;
	while (i < n && (us1[i] || us2[i]))
	{
		if (to_lower(us1[i]) != to_lower(us2[i]))
			return (to_lower(us1[i]) - to_lower(us2[i]));
		i++;
	}
	return (0);
}

t_bool	is_smaller(t_dlst *node_a, t_dlst *node_b)
{
	char	*node_a_str;
	char	*node_b_str;
	size_t	node_a_len;
	size_t	node_b_len;
	size_t	len;

	node_a_str = get_toklist_str(node_a);
	node_b_str = get_toklist_str(node_b);
	node_a_len = ft_strlen(node_a_str);
	node_b_len = ft_strlen(node_b_str);
	if (node_a_len > node_b_len)
		len = node_a_len;
	else
		len = node_b_len;
	if (ft_strncase(node_a_str, node_b_str, len) < 0)
		return (true);
	else
		return (false);
}
