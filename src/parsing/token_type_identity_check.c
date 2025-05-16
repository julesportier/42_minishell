/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token_type_identity_check.c                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: juportie <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/28 16:10:01 by juportie          #+#    #+#             */
/*   Updated: 2025/05/15 11:03:48 by juportie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"
#include "parsing.h"

t_bool	is_control_op(enum e_token_type type)
{
	return (type == or
			|| type == and);
}

t_bool	is_pipeline_op(enum e_token_type type)
{
	return (type == pipeline);
}

t_bool	is_redir_op(enum e_token_type type)
{
	return (type == redir_output
			|| type == append_output
			|| type == redir_input
			|| type == heredoc);
}

t_bool	is_grouping_op(enum e_token_type type)
{
	return (type == left_parenthesis
			|| type == right_parenthesis);
}

t_bool	is_primary(enum e_token_type type)
{
	return (type == literal
			|| type == double_quotes
			|| type == variable
			|| type == wildcard);
}
