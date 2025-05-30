/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expansions.h                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: juportie <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/26 10:25:08 by juportie          #+#    #+#             */
/*   Updated: 2025/05/30 16:21:50 by juportie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef EXPANSIONS_H
# define EXPANSIONS_H

# include "../../libft/src/libft.h"
# include "../minishell.h"
# include "../parsing/parsing.h"

// expansions_utils.c
t_error	place_expansion_result(
	t_dlst **token,
	enum e_token_type type,
	char *new_str,
	t_error *error);
char	*join_char_free(char *str, char c);
// expansions_toklist_utils.c
t_error	replace_token_content(
	t_token *token,
	enum e_token_type type,
	char *str,
	t_error *error);
t_dlst	*insert_expanded_token(t_dlst *token, char *str, t_error *error);
void	remove_token(t_dlst **token, t_dlst **toklist);
t_error	concatenate_toklist(t_dlst **toklist, t_error *error);
// expansions_variable.c
t_dlst	*expand_variable(
	t_dlst *token,
	t_dlst **toklist,
	t_shell_vars *shell_vars,
	t_error *error);
// expansions_double_quotes.c
t_dlst	*expand_double_quotes(
	t_dlst *token,
	t_dlst **toklist,
	t_shell_vars *shell_vars,
	t_error *error);
// expansions_wildcard.c
t_dlst	*expand_wildcard(t_dlst *token, t_dlst **toklist, t_error *error);
// heredoc_to_file.c
t_error	populate_heredocs_files(t_dlst *toklist, t_error *error);
// expansions.c
t_error	expand_toklist(t_dlst **toklist, t_shell_vars *shell_vars);

#endif
