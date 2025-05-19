/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   build_tree.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: juportie <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/06 09:44:07 by juportie          #+#    #+#             */
/*   Updated: 2025/05/16 13:04:24 by juportie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>
#include "../../libft/src/libft.h"
#include "../minishell.h"
#include "parsing.h"
#include "build_tree.h"




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

// FIX NODE MOVING FROM TOKLIST TO TREE_NODE AND HANDLE ERRORS
//static t_dlst	*set_redirection(t_bin_tree *tree_node, t_dlst *toklist)
//{
//	int	nesting_level;
//
//	nesting_level = 0;
//	while (toklist)
//	{
//		nesting_level = update_parenthesis_nesting_level(toklist, nesting_level);
//		if (is_redir_op(get_toklist_type(toklist)) && nesting_level == 0)
//		{
//			if (get_toklist_type(toklist) == redir_input
//				|| get_toklist_type(toklist) == heredoc)
//				ft_dlstadd_back(&(tree_node->content->inputs), toklist);
//			else
//				ft_dlstadd_back(&(tree_node->content->outputs), toklist);
//			toklist = toklist->next;
//			ft_dlstremove(toklist->prev, NULL, NULL);
//		}
//		else
//			toklist = toklist->next;
//	}
//	return (toklist);
//}

static t_dlst	*find_operator(t_bin_tree *tree_node, t_dlst **toklist, t_error *error)
{
	t_dlst	*token;

	if (*toklist == NULL)
	{
		*error = recoverable;
		ft_putendl_fd("minishell: syntax error: illformed input", 2);
		return (NULL);
	}
	token = find_control_op(*toklist);//, error);
	if (token || *error)
		return (token);
	token = find_pipeline_op(*toklist);//, error);
	if (token || *error)
		return (token);
	//set_redirection(toklist, tree_node, error);
	//if (*error)
	//	return (NULL);
	token = extract_grouping_content(tree_node, toklist, error);
	if (*error || token == NULL)
		return (NULL);
	token = find_operator(tree_node, toklist, error);
	if (token || *error)
		return (token);
	return (NULL);
}

static t_error	free_tree_materials(t_dlst **toklist, t_bin_tree **tree_node, t_error *error)
{
	if (toklist)
	{
		free_toklist(toklist);
		*toklist = NULL;
	}
	if (tree_node && *tree_node)
	{
		free_tree(tree_node);
		*tree_node = NULL;
	}
	return (*error);
}

// GET RID OF INT RETURN VALUES AND JUST RETURN IF ERROR (REDUCE LINE NUMBER)
static t_error	populate_parse_tree(t_bin_tree **tree_node, t_dlst **toklist, t_error *error)
{
	t_dlst	*toklist_left;
	t_dlst	*toklist_right;
	t_dlst	*pivot;

	// PUT SEARCH_OPERATOR INTO DIVIDE_TOKENS_LIST ?
	pivot = find_operator(*tree_node, toklist, error);
	if (*error)
		return (free_tree_materials(toklist, tree_node, error));
	else if (pivot)
		(*tree_node)->operator = get_toklist_type(pivot);
	else
	{
		(*tree_node)->operator = null;
		(*tree_node)->content->tokens_list = *toklist;
		return (success);
	}
	(*tree_node)->left = alloc_tree_node(); // CHECK MALLOC;
	(*tree_node)->right = alloc_tree_node(); // CHECK MALLOC;
	if ((*tree_node)->left == NULL || (*tree_node)->right == NULL) 
	{
		*error = critical;
		return (free_tree_materials(toklist, tree_node, error));
	}
	(*tree_node)->left->parent = *tree_node;
	(*tree_node)->right->parent = *tree_node;
	if (divide_tokens_list(&toklist_left, &toklist_right, toklist, &pivot) != success)
	{
		*error = recoverable;
		free_tree_materials(NULL, tree_node, error);
		return (*error);
	}
	populate_parse_tree(&(*tree_node)->left, &toklist_left, error);
	populate_parse_tree(&(*tree_node)->right, &toklist_right, error);
	if (*error)
		return (free_tree_materials(NULL, tree_node, error));
	return (success);
}


// TOKLIST MUST NOT BE EMPTY
t_bin_tree	*build_parse_tree(t_dlst **toklist, t_error *error)
{
	t_bin_tree	*parse_tree;

	parse_tree = alloc_tree_node();
	if (parse_tree == NULL)
	{
		*error = critical;
		free_toklist(toklist);
		return (NULL);
	}
	if (populate_parse_tree(&parse_tree, toklist, error) != SUCCESS)
	{
		//free_tree(&parse_tree);
		return (NULL);
	}
	else
		return (parse_tree);
}

//int	main(int argc, char *argv[])
//{
//	t_error	error = success;
//
//	if (argc != 2)
//		return (-1);
//	t_dlst	*toklist = scan_line(argv[1], &error);
//	if (error)
//	{
//		printf("scanning error\n");
//		free_toklist(&toklist);
//		return (-1);
//	}
//	else if (!toklist)
//	{
//		printf("empty toklist\n");
//		return (0);
//	}
//	t_bin_tree	*tree = build_parse_tree(&toklist, &error);
//	if (error)
//	{
//		printf("parsing error\n");
//		print_tree(tree, 0);
//		return (-1);
//	}
//	print_tree(tree, 0);
//	free_tree(&tree);
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
