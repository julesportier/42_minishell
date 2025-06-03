/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expansions_wildcard.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: juportie <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/26 10:25:15 by juportie          #+#    #+#             */
/*   Updated: 2025/06/05 14:48:23 by juportie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <dirent.h>
#include <stdio.h>
#include <errno.h>
#include "../../libft/src/libft.h"
#include "../error_handling/errors.h"
#include "../parsing/parsing.h"
#include "../parsing/lexer.h"
#include "expansions.h"

// static t_dlst	*set_wildcard_to_literal(t_dlst *token, t_error *error)
// {
// 	if (replace_token_content((t_token *)(token->content), literal, "*", error)
// 			!= success)
// 		return (NULL);
// 	return (token);
// }

static t_dlst	*set_wildcard_to_literal(t_dlst *token)
{
	((t_token *)(token->content))->type = literal;
	return (token);
}

static t_dlst	*merge_wildcard(t_dlst *token, t_error *error)
{
	while (token->prev && get_toklist_cat_prev(token) == true)
		token = token->prev;
	set_toklist_type(token, wildcard);
	while (token
			&& token->next
			&& get_toklist_cat_prev(token->next) == true)
	{
		if (merge_next_token(token, error) != success)
			return (NULL);
	}
	return (token);
}

static t_error	append_entry_to_list(
	t_dlst **cwd_content,
	char *entry,
	t_error *error)
{
	t_token	*token;

	token = alloc_token(error);
	if (*error)
		return (*error);
	token->str = ft_strdup(entry);
	if (!token->str)
	{
		free(token);
		return (set_err_return_err_enun(error, critical));
	}
	token->type = literal;
	append_token_to_list(cwd_content, token, error);
	if (*error)
		free(token);
	return (*error);
}

static t_dlst	*get_cwd_content_list(
	DIR *dir_stream,
	t_bool match_directories_only,
	t_error *error)
{
	t_dlst			*cwd_content;
	struct dirent	*dir_stat;

	cwd_content = NULL;
	while (1)
	{
		errno = 0;
		dir_stat = readdir(dir_stream);
		if (errno)
		{
			perror("minishell: cwd read error");
			// CRITICAL ??
			*error = recoverable;
		}
		if (!dir_stat)
			break ;
		if (match_directories_only && dir_stat->d_type == DT_DIR)
			append_entry_to_list(&cwd_content, dir_stat->d_name, error);
		else if (!match_directories_only)
			append_entry_to_list(&cwd_content, dir_stat->d_name, error);
		if (*error)
			break ;
	}
	if (*error)
		free_toklist(&cwd_content);
	return (cwd_content);
}

static t_bool	is_out_of_cwd(char *wildcard_str)
{
	int	i;

	i = 0;
	while (wildcard_str[i] == '.' && wildcard_str[i+1] == '/')
	{
		++i;
		while (wildcard_str[i] == '/')
			++i;
	}
	while (wildcard_str[i])
	{
		if (wildcard_str[i] == '/')
		{
			while (wildcard_str[i] == '/')
				++i;
			if (wildcard_str[i] != '\0')
				return (true);
		}
		else
			++i;
	}
	return (false);
}

static t_bool	is_matching_directories(char *wildcard_str)
{
	if (wildcard_str[ft_strlen(wildcard_str) - 1] == '/')
		return (true);
	else
		return (false);
}

static t_bool	is_matching_hidden_files(char *wildcard_str)
{
	int	i;

	i = 0;
	while (wildcard_str[i] == '.' && wildcard_str[i+1] == '/')
	{
		++i;
		while (wildcard_str[i] == '/')
			++i;
	}
	if (wildcard_str[i] == '.')
		return (true);
	else
		return (false);
}

//static	search_string(
//	char *wildcard_str,
//	t_bool after_wildcard,
//	struct dirent **entry,
//	t_bool match_directory_only);
//{
//	if (after_wildcard)
//	{
//		while (*wildcard_str 
 
//static char	*match_end_(const char *str, const char *substr, size_t len)
//{
//	size_t	i;
//	size_t	substr_len;
//	char	*substr_start;
//
//	substr_len = ft_strlen(substr);
//	if (substr_len == 0 || substr_len > len)
//		return ((char *)str);
//	i = 0;
//	substr_start = NULL;
//	while (i < len && str[i])
//	{
//		if (substr[0] == str[i])
//		{
//			substr_start = &((char *)str)[i];
//			if (
//				ft_strncmp(&str[i], substr, substr_len) == 0
//				&& i + substr_len <= len
//			)
//				return (substr_start);
//		}
//		i++;
//	}
//	return (NULL);
//}

static int	get_len_until_star(char *shifted_wildcard_str)
{
	int	i;

	i = 0;
	while (*(shifted_wildcard_str + i))
	{
		if (*(shifted_wildcard_str + i) == '*')
			return (i);
		++i;
	}
	return (i);
}

// static t_bool	is_only_slashes(char *shifted_wildcard_str)
// {
// 	while (*shifted_wildcard_str == '/')
// 		++shifted_wildcard_str;
// 	if (*shifted_wildcard_str == '\0')
// 		return (true);
// 	else
// 		return (false);
// }

static t_bool	match_glob_substr(char *shifted_wildcard_str, char *shifted_entry)
{
	size_t	substr_len;

	substr_len = get_len_until_star(shifted_wildcard_str);
	if (substr_len > ft_strlen(shifted_entry))
		return (false);
	if (DEBUG)
	{
		printf("glob_substr len == %zo ; star starting %s entry start %s\n", substr_len, shifted_wildcard_str, shifted_entry);
	}
	if (ft_strncmp(shifted_wildcard_str, shifted_entry, substr_len))
		return (false);
	else
		return (true);
}


t_bool	match_glob_entry(char *wildcard_str, char *entry)
{
	int	i;
	int	glob_index;
	t_bool	after_wildcard;

	i = 0;
	// while (wildcard_str[i] == '.' && wildcard_str[i+1] == '/')
	// {
	// 	++i;
	// 	while (wildcard_str[i] == '/')
	// 		++i;
	// }
	//if (wildcard_str[i] == '.')
	//{
	//	hidden_files = true;
	//	++i;
	//}
	//else
	//	hidden_files = false;
	after_wildcard = false;
	glob_index = 0;
	while (wildcard_str[i])
	{
		while (wildcard_str[i] == '*')
		{
			after_wildcard = true;
			++i;
		}
		if (!wildcard_str[i])
			return (true);
		if (after_wildcard)
		{
			while (entry[glob_index])
			{
				if (match_glob_substr(&(wildcard_str[i]), &(entry[glob_index])))
				{
					glob_index += get_len_until_star(&wildcard_str[i]);
					i += get_len_until_star(&wildcard_str[i]);
					break ;
				}
				else
					++glob_index;
			}
		}
		else if (match_glob_substr(&(wildcard_str[i]), &(entry[glob_index])))
		{
			glob_index += get_len_until_star(&wildcard_str[i]);
			i += get_len_until_star(&wildcard_str[i]);
		}
		else
			return (false);
		after_wildcard = false;
	}
	return (true);
}

t_bool	is_hidden_file(t_dlst *token)
{
	return (get_toklist_str(token)[0] == '.');
}

t_bool	is_non_hidden_file(t_dlst *token)
{
	return (get_toklist_str(token)[0] != '.');
}

static void	remove_token(t_dlst **toklist, t_dlst *token)
{
	if (token->prev == NULL)
		*toklist = (*toklist)->next;
	ft_dlstremove(token, free_token_content, free);
}
// void	remove_token(t_dlst **toklist, t_dlst *token)
// {
// 	t_dlst	*node;
// 	t_dlst	*next;
//
// 	node = *toklist;
// 	while (node)
// 	{
// 		next = node->next;
// 		if (node == token)
// 		{
// 			if (node->prev == NULL)
// 				*toklist = (*toklist)->next;
// 			ft_dlstremove(node, free_token_content, free);
// 			return ;
// 		}
// 		node = next;
// 	}
// 	return ;
// }

t_dlst	*remove_token_if(t_bool test(t_dlst *token), t_dlst *cwd_content_list)
{
	t_dlst	*head;
	t_dlst	*next;

	head = cwd_content_list;
	while (cwd_content_list)
	{
		next = cwd_content_list->next;
		if (test(cwd_content_list))
		{
			if (cwd_content_list->prev == NULL)
				head = head->next;
			ft_dlstremove(cwd_content_list, free_token_content, free);
		}
		cwd_content_list = next;
	}
	return (head);
}

static size_t	get_junk_start_len(char *wildcard_str)
{
	size_t	i;

	i = 0;
	while (wildcard_str[i] == '.' && wildcard_str[i+1] == '/')
	{
		++i;
		while (wildcard_str[i] == '/')
			++i;
	}
	return i;
}

static size_t	get_junk_end_len(char *wildcard_str)
{
	size_t	len;
	size_t	i;

	i = 0;
	len = ft_strlen(wildcard_str);
	while (len)
	{
		if (wildcard_str[len] == '/')
			++i;
		else
			break ;
		--len;
	}
	return (i);
}

char	*trim_wildcard_str(char *wildcard_str, t_error *error)
{
	char	*str;
	
	str = ft_strdup(wildcard_str + get_junk_start_len(wildcard_str));
	if (!str)
		set_err_return_null(error, critical);
	str[ft_strlen(str) - 1 - get_junk_end_len(str)] = '\0';
	if (DEBUG)
		printf("trimmed str == %s\n", str);
	return (str);
}

t_dlst	*match_cwd_content_list(char *wildcard_str, t_dlst *cwd_content_list, t_error *error)
{
	t_dlst	*head;
	t_dlst	*next;
	char	*trimmed_wildcard_str;

	if ((is_matching_directories(wildcard_str) && ft_isequalstr(wildcard_str, "*/"))
		|| (is_matching_hidden_files(wildcard_str) && ft_isequalstr(wildcard_str, ".*")))
		return (cwd_content_list);
	head = cwd_content_list;
	trimmed_wildcard_str = trim_wildcard_str(wildcard_str, error);
	if (*error)
		return (NULL);
	while (cwd_content_list)
	{
		next = cwd_content_list->next;
		if (!match_glob_entry(trimmed_wildcard_str, get_toklist_str(cwd_content_list)))
		{
			if (cwd_content_list->prev == NULL)
				head = head->next;
			ft_dlstremove(cwd_content_list, free_token_content, free);
		}
		// DEBUG
		else
			printf("matched glob entry\n");
		//DEBUG
		cwd_content_list = next;
	}
	free(trimmed_wildcard_str);
	return (head);
}


t_dlst	*expand_wildcard_token(t_dlst **toklist, t_dlst *token, t_error *error)
{
	char	cwd[PATH_MAX];
	DIR		*dir_stream;
	t_dlst	*cwd_content_list;
	t_dlst	*sublist_last;

	if (is_out_of_cwd(get_toklist_str(token)))
		return (set_wildcard_to_literal(token));
	if (!getcwd(cwd, PATH_MAX))
		return (set_wildcard_to_literal(token));
	dir_stream = opendir(cwd);
	// IF DIRECTORY NOT EXIST OR USER DON'T HAVE PERMISSIONS BASH JUST RETURNS THE LITERAL '*'
	// CHECK IF ON OTHER ERRORS IT PRINT AN ERROR MESSGAGE END WHAT IT DOES
	if (!dir_stream)
		return (set_wildcard_to_literal(token));
	cwd_content_list = get_cwd_content_list(
		dir_stream, is_matching_directories(get_toklist_str(token)), error);
	closedir(dir_stream);
	if (cwd_content_list == NULL)
		return (token);
	if (DEBUG)
	{
		printf("- cwd_content_list before hidden files match (already checked if dir)\n");
		print_toklist(cwd_content_list, 1);
	}
	if (is_matching_hidden_files(get_toklist_str(token)))
		cwd_content_list = remove_token_if(is_non_hidden_file, cwd_content_list);
	else
		cwd_content_list = remove_token_if(is_hidden_file, cwd_content_list);
	if (cwd_content_list == NULL)
		return (set_wildcard_to_literal(token));
	if (DEBUG)
	{
		printf("- cwd_content_list before star match\n");
		print_toklist(cwd_content_list, 1);
	}
	cwd_content_list = match_cwd_content_list(get_toklist_str(token), cwd_content_list, error);
	if (DEBUG)
	{
		printf("- cwd_content_list after match\n");
		print_toklist(cwd_content_list, 0);
	}
	if (cwd_content_list)
	{
		// ? APPEND FIRST (. .. but ../ ./)
		// append_junk_to_results(cwd_content_list); -> ././//./      *    /
		sort_list(&cwd_content_list);
		sublist_last = ft_dlstlast(cwd_content_list);
		ft_dlstinsert_sublist_next(token, cwd_content_list);
		remove_token(toklist, token);
		return (sublist_last);
	}
	return (token);
	// return sublist_last or input token if list is NULL
}

static t_error	concatenate_wildcards(t_dlst **toklist, t_error *error)
{
	t_dlst 			*token;

	token = *toklist;
	while (token)
	{
		if (get_toklist_type(token) == wildcard)
		{
			token = merge_wildcard(token, error);
			if (*error)
				return (*error);
		}
		token = token->next;
		if (token == NULL)
			break ;
	}
	return (success);
}

t_dlst	*expand_wildcards(t_dlst **toklist, t_error *error)
{
	t_dlst 			*token;

	if (concatenate_wildcards(toklist, error) != success)
		return (NULL);
	token = *toklist;
	while (token)
	{
		if (DEBUG)
			printf("check if expanding token: '%s'...\n", get_toklist_str(token));
		if (get_toklist_type(token) == wildcard)
		{
			if (DEBUG)
				printf("expanding token: '%s'...\n", get_toklist_str(token));
			token = expand_wildcard_token(toklist, token, error);
			if (*error)
				return (NULL);
		}
		token = token->next;
	}

	//while (1)
	//{
	//	dir_stat = readdir(dir_stream);
	//	if (!dir_stat)
	//		break ;
	//	//printf("readdir-> `%s'\n", dir_stat->d_name);
	//	if (dir_stat->d_name[0] != '.')
	//	{
	//		if (place_expansion_result(&token, wildcard, dir_stat->d_name, error) != success)
	//			return (NULL);
	//	}
	//}
	//closedir(dir_stream);
	//if (get_toklist_type(token) == wildcard)
	//	return (set_wildcard_to_literal(token, error));
	return (token);
}



//static t_error	dirent_to_token(t_dlst 	*dirent_node, t_error *error)
//{
//	struct dirent	*entry;
//	t_token	*token;
//
//	token = alloc_token(error);
//	if (*error)
//		return (*error);
//	entry = (struct dirent *)(dirent_node->content);
//	token->str = ft_strdup(entry->d_name);
//	free(dirent_node->content);
//	if (!token->str)
//	{
//		free(token);
//		return (set_err_return_err_enun(error, critical));
//	}
//	token->type = literal;
//	dirent_node->content = token;
//	return (success);
//}

// static t_error	add_cwd_entry(char *entry, t_dlst **list, t_error *error)
// {
// 	t_token	*token;
//
// 	token = alloc_token(error);
// 	if (*error)
// 		return (*error);
// 	token->type = literal;
// 	token->str = ft_strdup(entry);
// 	if (token->str == NULL)
// 	{
// 		*error = critical;
// 		free(token);
// 		return (*error);
// 	}
// 	*error = append_token_to_list(list, token, error);
// 	if (*error)
// 	{
// 		free(token->str);
// 		free(token);
// 	}
// 	return (*error);
// }
// static t_error	add_cwd_entry(struct dirent *dir_stat, t_dlst **list, t_error *error)
// {
// 	struct dirent	*content;
// 	t_dlst	*new_node;
//
// 	content = malloc(sizeof(struct dirent));
// 	if (content == NULL)
// 		return (set_err_return_err_enun(error, critical));
// 	content = ft_memcpy(content, dir_stat, sizeof(struct dirent));
// 	new_node = ft_dlstnew(content);
// 	if (new_node == NULL)
// 	{
// 		free(content);
// 		return (set_err_return_err_enun(error, critical));
// 	}
// 	ft_dlstadd_back(list, new_node);
// 	return (*error);
// }
