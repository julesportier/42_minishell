/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokens_list_print.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: juportie <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/06 10:59:37 by juportie          #+#    #+#             */
/*   Updated: 2025/05/13 11:31:56 by juportie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>
#include "parsing.h"

const char	*token_type_to_str(enum e_token_type type)
{
	const char	*token_types_lookup[16] = {
		"null",
		"literal",
		"double_quotes",
		"variable",
		"wildcard",
		"or",
		"and",
		"pipeline",
		"left_parenthesis",
		"right_parenthesis",
		"redir_output",
		"append_output",
		"redir_input",
		"heredoc",
		"heredoc_exp",
		"heredoc_lit"
	};

	if (type >= 0 && type <= 16)
		return (token_types_lookup[type]);
	else
		return ("non identified");
}

void	print_toklist(t_dlst *list, int indent)
{
	int	i;

	i = 0;
	while (list)
	{
		if (list->content)
		{
			print_indent(indent);
			printf("token %d: %s, '%s', cat_prev: %s\n",
				i,
				token_type_to_str(get_toklist_type(list)),
				get_toklist_str(list),
				bool_to_str(get_toklist_cat_prev(list)));
		}
		else
		{
			print_indent(indent);
			printf("token %d: content == NULL\n", i);
		}
		list = list->next;
		++i;
	}
}
