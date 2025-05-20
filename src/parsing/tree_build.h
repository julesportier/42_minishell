/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tree_build.h                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: juportie <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/06 09:44:07 by juportie          #+#    #+#             */
/*   Updated: 2025/05/20 11:39:25 by juportie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../libft/src/libft.h"
#include "../minishell.h"
#include "parsing.h"

// token_type_identity_check.c
t_bool	is_control_op(enum e_token_type type);
t_bool	is_pipeline_op(enum e_token_type type);
t_bool	is_redir_op(enum e_token_type type);
t_bool	is_grouping_op(enum e_token_type type);
t_bool	is_primary(enum e_token_type type);
// tree_build_binary_op.c
t_dlst	*find_control_op(t_dlst *toklist);//, t_error *error)
t_dlst	*find_pipeline_op(t_dlst *toklist);//, t_error *error)
t_error	divide_tokens_list(
	t_dlst	**toklist_left,
	t_dlst 	**toklist_right,
	t_dlst	**toklist,
	t_dlst	**pivot);
// tree_build_redirections.c
t_error	set_redirection(t_bin_tree *tree_node, t_dlst **toklist);
// tree_build_grouping.c
t_dlst	*extract_grouping_content(t_bin_tree *tree_node, t_dlst **toklist, t_error *error);
