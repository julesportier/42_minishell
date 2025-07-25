/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirs_expansion_utils.c                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ecasalin <ecasalin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/07 09:23:28 by ecasalin          #+#    #+#             */
/*   Updated: 2025/06/13 10:55:14 by ecasalin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <unistd.h>
#include "../minishell.h"
#include "../parsing/parsing.h"
#include "../error_handling/errors.h"
#include "../expansions/expansions.h"

static int	is_word(t_dlst *temp_head)
{
	if (temp_head == NULL)
		return (0);
	if (get_toklist_type(temp_head) == literal)
		return (1);
	if (get_toklist_type(temp_head) == heredoc_exp)
		return (1);
	if (get_toklist_type(temp_head) == heredoc_lit)
		return (1);
	return (0);
}

int	check_redir_list(t_dlst *temp_head, t_error *error)
{
	if (temp_head != NULL && is_word(temp_head))
		return (set_err_return_err(error, recoverable));
	while (temp_head != NULL)
	{
		if (!is_word(temp_head) && temp_head->next == NULL)
			return (set_err_return_err(error, recoverable));
		if (temp_head->next != NULL)
		{
			if (is_word(temp_head) && is_word(temp_head->next))
				return (set_err_return_err(error, recoverable));
			if (!is_word(temp_head) && !is_word(temp_head->next))
				return (set_err_return_err(error, recoverable));
		}
		temp_head = temp_head->next;
	}
	return (SUCCESS);
}

static int	check_redirs_expansion(t_bin_tree *curr_node, t_error *error)
{
	if (check_redir_list(curr_node->content->inputs, error) == ERROR)
		return (return_error("minishell: syntax error: ill-formed redirection, "
				"unexpected `redir_input'\n", ERROR));
	if (check_redir_list(curr_node->content->outputs, error) == ERROR)
		return (return_error("minishell: syntax error: ill-formed redirection, "
				"unexpected `redir_output'\n", ERROR));
	return (SUCCESS);
}

int	expand_redirections(t_bin_tree *curr_node, t_shell_vars *vars,
						t_error *error)
{
	if (expand_toklist(&curr_node->content->inputs, vars, error))
		return (ERROR);
	if (expand_toklist(&curr_node->content->outputs, vars, error))
		return (ERROR);
	if (check_redirs_expansion(curr_node, error) == ERROR)
		return (ERROR);
	return (SUCCESS);
}

int	perform_expansions(t_bin_tree *curr_node, t_shell_vars *vars,
						t_error *error)
{
	if (expand_redirections(curr_node, vars, error) == ERROR)
		return (ERROR);
	if (expand_toklist(&curr_node->content->tokens_list, vars, error))
		return (ERROR);
	return (SUCCESS);
}
