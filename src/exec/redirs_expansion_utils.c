/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirs_expansion_utils.c                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ecasalin <ecasalin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/07 09:23:28 by ecasalin          #+#    #+#             */
/*   Updated: 2025/06/07 11:08:48 by ecasalin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <unistd.h>
#include "../minishell.h"
#include "../parsing/parsing.h"
#include "../error_handling/errors.h"
#include "../expansions/expansions.h"

static int	iterate_redir_list(t_dlst *temp_head, t_error *error)
{
	if (temp_head != NULL && get_toklist_type(temp_head) == literal)
		return (set_err_return_err(error, recoverable));
	while (temp_head != NULL)
	{
		if (get_toklist_type(temp_head) != literal &&
			get_toklist_type(temp_head) != heredoc_exp &&
			get_toklist_type(temp_head) != heredoc_lit &&
			temp_head->next == NULL)
			return (set_err_return_err(error, recoverable));
		if (temp_head->next != NULL)
		{
			if (get_toklist_type(temp_head) != literal &&
				get_toklist_type(temp_head->next) != literal)
				return (set_err_return_err(error, recoverable));
			if (get_toklist_type(temp_head) == literal &&
				get_toklist_type(temp_head->next) == literal)
				return (set_err_return_err(error, recoverable));
		}
		temp_head = temp_head->next;
	}
	return (SUCCESS);
}

static int	check_redirs_expansion(t_bin_tree *curr_node, t_error *error)
{
	if (iterate_redir_list(curr_node->content->inputs, error) == ERROR)
		return (return_error("minishell: syntax error: ill-formed redirection, "
				"unexpected `redir_input'\n", ERROR));
	if (iterate_redir_list(curr_node->content->outputs, error) == ERROR)
		return (return_error("minishell: syntax error: ill-formed redirection, "
				"unexpected `redir_output'\n", ERROR));
	return (SUCCESS);
}

int	expand_redirections(t_bin_tree *curr_node, t_shell_vars *vars,
						t_error *error)
{
	expand_toklist(&curr_node->content->inputs, vars);
	expand_toklist(&curr_node->content->outputs, vars);
	if (check_redirs_expansion(curr_node, error) == ERROR)
		return (ERROR);
	return (SUCCESS);
}

int	perform_expansions(t_bin_tree *curr_node, t_shell_vars *vars,
						t_error *error)
{
	if (expand_redirections(curr_node, vars, error) == ERROR)
		return (ERROR);
	expand_toklist(&curr_node->content->tokens_list, vars);
	return (SUCCESS);
}
