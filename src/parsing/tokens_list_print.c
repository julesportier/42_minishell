/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokens_list_print.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: juportie <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/06 10:59:37 by juportie          #+#    #+#             */
/*   Updated: 2025/05/06 12:25:47 by juportie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>
#include "parsing.h"

static const char	*tokens_types_str[] = {
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
	"heredoc"
};

static const char	*t_bool_str[] = {
	"false",
	"true"
};

void	print_toklist(t_dlst *list)
{
	int	i;

	i = 0;
	while (list != NULL)
	{
		if (list->content != NULL)
		{
			printf("token %d: %s, '%s', cat_prev: %s\n",
				i,
				tokens_types_str[get_toklist_type(list)],
				get_toklist_str(list),
				t_bool_str[get_toklist_cat_prev(list)]);
		}
		else
			printf("token %d: content == NULL\n", i);
		list = list->next;
		++i;
	}
}
//
//int main(void)
//{
//	t_dlst	*tokens_list;
//	t_error	error;
//
//	tokens_list = scan_line("echo $var | \"cat\" \'", &error);
//	print_toklist(tokens_list);
//	return (SUCCESS);
//}
