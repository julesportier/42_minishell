/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expansions.h                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: juportie <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/26 10:25:08 by juportie          #+#    #+#             */
/*   Updated: 2025/06/09 17:45:32 by juportie         ###   ########.fr       */
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
char	*join_char_free(char *str, char c, t_error *error);
// expansions_toklist_utils.c
t_error	replace_token_content(
			t_token *token,
			enum e_token_type type,
			char *str,
			t_error *error);
t_error	merge_next_token(t_dlst *token, t_error *error);
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
			t_shell_vars *shell_vars,
			t_error *error);
// expansions_wildcard_identity_check.c
t_bool	is_out_of_cwd(char *wildcard_str);
t_bool	is_matching_directories(char *wildcard_str);
t_bool	is_matching_hidden_files(char *wildcard_str);
t_bool	is_hidden_file(t_dlst *token);
t_bool	is_non_hidden_file(t_dlst *token);
// expansions_wildcard_sort_cmp.c
t_bool	is_smaller(t_dlst *node_a, t_dlst *node_b);
// expansions_wildcard_sort.c
void	sort_list(t_dlst **cwd_content_list);
// expansions_wildcard_junk_utils.c
size_t	get_junk_start_len(char *wildcard_str);
size_t	get_junk_end_len(char *wildcard_str);
char	*trim_wildcard_str(char *wildcard_str, t_error *error);
// expansions_wildcard_append_junk.c
t_error	append_junk_to_cwd_list(
			t_dlst	*cwd_content_list,
			char	*wildcard_str,
			t_error	*error);
// expansions_wildcard_create_cwd_list.c
t_dlst	*create_filtered_cwd_content_list(t_dlst *token, t_error *error);
// expansions_wildcard_glob_utils.c
int		get_len_until_star(char *shifted_wildcard_str);
int		skip_wildcards(char *wildcard_str, int i, t_bool *after_wildcard);
void	increment_indexes(int *i, int *glob_index, char *shifted_wildcard_str);
// expansions_wildcard_glob.c
t_bool	match_glob_entry(char *wildcard_str, char *entry);
// expansions_wildcard_concatenate.c
t_error	concatenate_wildcards(t_dlst **toklist, t_error *error);
// expansions_wildcard.c
void	sort_list(t_dlst **cwd_content_list);
t_dlst	*expand_wildcards(t_dlst **toklist, t_error *error);
// heredoc_to_file.c
t_error	populate_heredocs_files(t_dlst *toklist, t_error *error);
// expansions.c
t_error	expand_toklist(
			t_dlst **toklist,
			t_shell_vars *shell_vars,
			t_error *error);

#endif
