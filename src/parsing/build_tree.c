/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   build_tree.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: juportie <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/06 09:44:07 by juportie          #+#    #+#             */
/*   Updated: 2025/05/09 17:28:13 by juportie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../libft/src/libft.h"
#include "../minishell.h"
#include "parsing.h"

static t_bin_tree	*alloc_tree_node(void)
{
	t_bin_tree	*node;
	t_node_content	*content;

	node = ft_calloc(1, sizeof(t_bin_tree));
	if (node == NULL)
		return (NULL);
	content = ft_calloc(1, sizeof(t_node_content));
	if (content == NULL)
	{
		free(node);
		return (NULL);
	}
	return (node);
}

static int	divide_tokens_list(
		t_node_content	*left,
		t_node_content 	*right,
		t_dlst	*toklist,
		t_dlst	*pivot)
{
	if (toklist == pivot)
	{
		return (print_syntax_error("missing operand before ", get_toklist_type(pivot), ERROR));
	}
	if (ft_dlstlast(toklist) == pivot)
	{
		return (print_syntax_error("missing operand after ", get_toklist_type(pivot), ERROR));
	}
	right->tokens_list = (pivot->next);
	ft_dlstsplit(pivot);
	left->tokens_list = toklist;
	return (SUCCESS);
}

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
	return (is_grouping_op(type)
			|| type == literal
			|| type == double_quotes
			|| type == variable
			|| type == wildcard);
}



/****************************
FUNCTIONS TO SEARCH OPERATORS
****************************/
//static t_dlst	*search_redir_op(t_dlst *toklist, t_error *error)
//{
//	while (toklist != NULL)
//	{
//		toklist = skip_parenthesis(toklist, error);
//		if (toklist == NULL)
//			return (NULL);
//		if (is_redir_op(get_toklist_type(toklist)))
//		{
//			if (is_grouping_op(get_toklist_type(toklist->next))
//				|| toklist->next == NULL)
//			{
//				*error = recoverable;
//				print_syntax_error("can't have subshell after redirection ", get_toklist_type(toklist), ERROR);
//			}
//			return (toklist);
//		}
//		toklist = toklist->next;
//	}
//	if (!error)
//		return (search_???_op(ft_dlsthead(toklist), error));
//	return (NULL);
//}
//static t_dlst	*search_pipeline_op(t_dlst *toklist, t_error *error)
//{
//	while (toklist != NULL)
//	{
//		toklist = skip_parenthesis(toklist, error);
//		if (toklist == NULL)
//			return (NULL);
//		if (is_pipeline_op(get_toklist_type(toklist)))
//			return (toklist);
//		toklist = toklist->next;
//	}
//	if (!error)
//		return (search_redir_op(ft_dlsthead(toklist), error));
//	return (NULL);
//}
static int	update_parenthesis_nesting_level(t_dlst *token, int nesting_level)
{
	if (get_toklist_type(token) == right_parenthesis)
		return (--nesting_level);
	else if (get_toklist_type(token) == left_parenthesis)
		return (++nesting_level);
	else
		return (nesting_level);
}

static int	check_nesting_level(int nesting_level, t_dlst *toklist)
{
	if (nesting_level < 0 || (toklist->next == NULL && nesting_level > 0))
		return (ERROR);
	else
		return (SUCCESS);
}

static t_dlst	*find_binary_op(
	t_dlst *toklist,
	t_bool check_operator(enum e_token_type),
	t_error *error)
{
	static int	nesting_level;

	while (toklist)
	{
		if (toklist->prev == NULL)
			nesting_level = 0;
		nesting_level = update_parenthesis_nesting_level(toklist, nesting_level);
		if (check_nesting_level(nesting_level, toklist) == false)
		{
			*error = recoverable;
			return (NULL);
		}
		else if (check_operator(get_toklist_type(toklist)) && nesting_level == 0)
			return (toklist);
	}
	return (NULL);
}

static t_dlst	*find_control_op(t_dlst *toklist, t_error *error)
{
	return (find_binary_op(toklist, is_control_op, error));
}
static t_dlst	*find_pipeline_op(t_dlst *toklist, t_error *error)
{
	return (find_binary_op(toklist, is_pipeline_op, error));
}

static t_dlst	*find_operator(t_dlst *toklist, t_error *error)
{
	t_dlst	*head;

	head = toklist;
	toklist = find_control_op(head, error);
	if (*error)
		return (NULL);
	else if (toklist)
		return (toklist);
	toklist = find_operator(head, error);
	if (*error)
		return (NULL);
	else if (toklist)
		return (toklist);
	toklist = find_redirection(head, error);
	if (*error)
		return (NULL);
	else if (toklist)
		return (toklist);
	toklist = find_grouping(head, error);
	if (*error)
		return (NULL);
	return (toklist);
}




// GET RID OF INT RETURN VALUES AND JUST RETURN IF ERROR (REDUCE LINE NUMBER)
static int	populate_parse_tree(t_bin_tree *tree_node, t_error *error)
{
	t_bin_tree	*left;
	t_bin_tree	*right;
	t_dlst	*pivot;

	if (error)
		return (*error);
	pivot = NULL;
	// PUT SEARCH_OPERATOR INTO DIVIDE_TOKENS_LIST ?
	pivot = find_operator(tree_node->content->tokens_list, error);
	if (error)
		return (*error);
	if (pivot != NULL)
		tree_node->operator = get_toklist_type(pivot);
	else
		tree_node->operator = null;
	// If no operator found we are in a terminal expression
	// we can stop the recursion.
	if (tree_node->operator == null)
		return (SUCCESS);
	left = alloc_tree_node(); // CHECK MALLOC;
	right = alloc_tree_node(); // CHECK MALLOC;
	if (left == NULL || right == NULL) 
	{
		// FREE TOUT !!!!
		// free tree (toklist included inside)
		*error = critical;
		return (CRIT_ERROR);
	}
	divide_tokens_list(
		left->content, right->content,
		tree_node->content->tokens_list, pivot);
	if (populate_parse_tree(left, error) != SUCCESS)
		return (*error);
	if (populate_parse_tree(right, error) != SUCCESS)
		return (*error);
	return (SUCCESS);
}

static int	check_parenthesis_structure(t_dlst *toklist)
{
	static int	nesting_level;

	if (toklist->prev == NULL)
		nesting_level = 0;
	while (toklist)
	{
		nesting_level = update_parenthesis_nesting_level(toklist, nesting_level);
		if (check_nesting_level(nesting_level, toklist) == ERROR)
			return (ERROR);
		toklist = toklist->next;
	}
	return (SUCCESS);
}

t_bin_tree	*build_parse_tree(t_dlst *toklist, t_error *error)
{
	t_bin_tree	*parse_tree;

	// The caller must handle 
	if (check_parenthesis_structure(toklist) == ERROR)
		return (NULL);
	parse_tree = alloc_tree_node();
	if (parse_tree == NULL)
	{
		*error = critical;
		ft_dlstclear(toklist, free_token_content);
		return (NULL);
	}
	parse_tree->content->tokens_list = toklist;
	populate_parse_tree(parse_tree, error);
	return (parse_tree);
}

//
//
//
//
//
//static t_dlst	*search_binary_op(t_dlst *toklist, t_error *error)
//{
//	while (toklist != NULL)
//	{
//		toklist = skip_parenthesis(toklist, error);
//		if (toklist == NULL)
//			return (NULL);
//		if (is_binary_op(get_toklist_type(toklist)))
//			return (toklist);
//		toklist = toklist->next;
//	}
//	if (!error)
//		return (search_pipeline_op(ft_dlsthead(toklist), error));
//	return (NULL);
//}


//
//static t_dlst	*close_parenthesis(t_dlst *toklist, t_error *error)
//{
//	int	nesting_level;
//
//	nesting_level = 0;
//	while (toklist->next)
//	{
//		toklist = toklist->next;
//		if (get_toklist_type(toklist) == right_parenthesis)
//		{
//			if (nesting_level == 0)
//				return (toklist);
//			else
//				--nesting_level;
//		}
//		if (get_toklist_type(toklist) == left_parenthesis)
//			++nesting_level;
//	}
//	*error = recoverable;
//	print_syntax_error("missing closing parenthesis after ", get_toklist_type(toklist), ERROR);
//	return (NULL);
//}
//
//// This function checks if there is
//static t_dlst	*skip_parenthesis(t_dlst *toklist, t_error *error)
//{
//	if (toklist->prev
//		&& is_grouping_op(get_toklist_type(toklist))
//		&& is_grouping_op(get_toklist_type(toklist->prev)))
//	{
//		*error = recoverable;
//		// OR JUST "SYNTAX ERROR NEAR UNEXPECTED TOKEN 'toktype'" -> ()) is an error
//		print_syntax_error("missing operator between parenthesis ", get_toklist_type(toklist), ERROR);
//	}
//	if (get_toklist_type(toklist) == right_parenthesis)
//	{
//		*error = recoverable;
//		print_syntax_error("missing opening parenthesis before ", get_toklist_type(toklist), ERROR);
//	}
//	toklist = close_parenthesis(toklist, error);
//	if (toklist != NULL)
//		toklist = toklist->next;
//	return (toklist);
//}
//static t_dlst	*skip_primary(t_dlst *toklist, t_error *error)
//{
//	while (is_primary(toklist))
//	{
//		toklist = skip_parenthesis(toklist, error);
//		if (*error)
//			return (NULL);
//
//
//	if (is_primary(get_toklist_type(toklist)))
//	{
//		*error = recoverable;
//		print_syntax_error("missing opening parenthesis before ", get_toklist_type(toklist), ERROR);
//	}
//	toklist = close_parenthesis(toklist, error);
//	if (toklist != NULL)
//		toklist = toklist->next;
//	return (toklist);
//}
