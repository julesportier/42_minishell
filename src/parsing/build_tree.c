/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   build_tree.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: juportie <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/06 09:44:07 by juportie          #+#    #+#             */
/*   Updated: 2025/05/13 17:24:37 by juportie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>
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
	node->content = content;
	return (node);
}

static int	divide_tokens_list(
		t_dlst	**toklist_left,
		t_dlst 	**toklist_right,
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
	*toklist_right = (pivot->next);
	ft_dlstsplit(pivot);
	*toklist_left = toklist;
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
	return (type == literal
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
	t_bool fptr_is_operator(enum e_token_type))
//	t_error *error)
{
	int	nesting_level;
	t_dlst	*operator;

	nesting_level = 0;
	operator = NULL;
	while (toklist)
	{
		nesting_level = update_parenthesis_nesting_level(toklist, nesting_level);
		if (fptr_is_operator(get_toklist_type(toklist)) && nesting_level == 0)
			operator = toklist;
		toklist = toklist->next;
	}
	return (operator);
}

// KEEP IT TO HANDLE PARENTHESIS
//
//static t_dlst	*extract_group(t_dlst *toklist)//, t_error *error)
//{
//	int	nesting_level;
//	//int	format_is_ok;
//
//	nesting_level = 0;
//	//if (get_toklist_type(toklist) == left_parenthesis)
//	//	format_is_ok = 1;
//	while (toklist)
//	{
//		nesting_level = update_parenthesis_nesting_level(toklist, nesting_level);
//		//if (nesting_level != 0 && !format_is_ok)
//		//{
//		//	*error = recoverable;
//		//	return (NULL);
//		//}
//		if (get_toklist_type(toklist) && nesting_level == 0 )
//			return (toklist);
//		toklist = toklist->next;
//	}
//	return (NULL);
//}

static t_dlst	*find_control_op(t_dlst *toklist)//, t_error *error)
{
	return (find_binary_op(toklist, is_control_op));//, error));
}
static t_dlst	*find_pipeline_op(t_dlst *toklist)//, t_error *error)
{
	return (find_binary_op(toklist, is_pipeline_op));//, error));
}


// FIX NODE MOVING FROM TOKLIST TO TREE_NODE AND HANDLE ERRORS
static t_dlst	*set_redirection(t_bin_tree *tree_node, t_dlst *toklist)
{
	int	nesting_level;

	nesting_level = 0;
	while (toklist)
	{
		nesting_level = update_parenthesis_nesting_level(toklist, nesting_level);
		if (is_redir_op(get_toklist_type(toklist)) && nesting_level == 0)
		{
			if (get_toklist_type(toklist) == redir_input
				|| get_toklist_type(toklist) == heredoc)
				ft_dlstadd_back(&(tree_node->content->inputs), toklist);
			else
				ft_dlstadd_back(&(tree_node->content->outputs), toklist);
			toklist = toklist->next;
			ft_dlstremove(toklist->prev, NULL, NULL);
		}
		else
			toklist = toklist->next;
	}
	return (toklist);
}

static t_dlst	*find_operator(t_bin_tree *tree_node, t_dlst *toklist, t_error *error)
{
	t_dlst	*head;

	head = toklist;
	toklist = find_control_op(head);//, error);
	if (toklist || *error)
		return (toklist);
	toklist = find_pipeline_op(head);//, error);
	if (toklist || *error)
		return (toklist);
	toklist = set_redirection(tree_node, head);
	//if (extract_grouping(&ft_dlsthead(toklist), error))
	//	find_operator(tree_node, toklist, error);
	//else
	return (NULL);
}

static int	free_tree_materials(t_dlst **toklist, t_bin_tree **tree_node, t_error *error)
{
	free_toklist(toklist);
	*toklist = NULL;
	free_tree(tree_root(*tree_node));
	*tree_node = NULL;
	return (*error);
}

// GET RID OF INT RETURN VALUES AND JUST RETURN IF ERROR (REDUCE LINE NUMBER)
static int	populate_parse_tree(t_bin_tree **tree_node, t_dlst **toklist, t_error *error)
{
	t_dlst	*toklist_left;
	t_dlst	*toklist_right;
	t_dlst	*pivot;

	// PUT SEARCH_OPERATOR INTO DIVIDE_TOKENS_LIST ?
	if (*error)
		return (free_tree_materials(toklist, tree_node, error));
	pivot = find_operator(*tree_node, *toklist, error);
	if (*error)
		return (free_tree_materials(toklist, tree_node, error));
	else if (pivot)
		(*tree_node)->operator = get_toklist_type(pivot);
	else
	{
		(*tree_node)->operator = null;
		(*tree_node)->content->tokens_list = *toklist;
		return (SUCCESS);
	}
	(*tree_node)->left = alloc_tree_node(); // CHECK MALLOC;
	(*tree_node)->right = alloc_tree_node(); // CHECK MALLOC;
	if ((*tree_node)->left == NULL || (*tree_node)->right == NULL) 
	{
		*error = critical;
		return (free_tree_materials(toklist, tree_node, error));
	}
	divide_tokens_list(&toklist_left, &toklist_right, *toklist, pivot);
	populate_parse_tree(&(*tree_node)->left, &toklist_left, error);
	populate_parse_tree(&(*tree_node)->right, &toklist_right, error);
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
		{
			print_syntax_error("unexpected ", get_toklist_type(toklist), ERROR);
			return (ERROR);
		}
		toklist = toklist->next;
	}
	return (SUCCESS);
}



// TOKLIST MUST NOT BE EMPTY
t_bin_tree	*build_parse_tree(t_dlst *toklist, t_error *error)
{
	t_bin_tree	*parse_tree;

	if (check_parenthesis_structure(toklist) == ERROR)
		return (NULL);
	parse_tree = alloc_tree_node();
	if (parse_tree == NULL)
	{
		*error = critical;
		free_toklist(&toklist);
		return (NULL);
	}
	parse_tree->content->tokens_list = toklist;
	if (populate_parse_tree(&parse_tree, &toklist, error))
		return (NULL);
	else
		return (parse_tree);
}

//int	main(int argc, char *argv[])
//{
//	t_error	error = success;
//
//	if (argc != 2)
//		return (FAILURE);
//	t_dlst	*toklist = scan_line(argv[1], &error);
//	if (error)
//	{
//		printf("scanning error\n");
//		return (-1);
//	}
//	t_bin_tree	*tree = build_parse_tree(toklist, &error);
//	if (error)
//	{
//		printf("parsing error\n");
//		return (-1);
//	}
//	print_tree(tree, 0);
//	return (0);
//}


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
