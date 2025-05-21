/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirections_utils.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ecasalin <ecasalin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/21 08:35:28 by ecasalin          #+#    #+#             */
/*   Updated: 2025/05/21 16:17:42 by ecasalin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include "../parsing/parsing.h"
#include "../error_handling/errors.h"
#include "exec.h"

int	set_output(t_bin_tree *curr_node)
{
	int		fd;
	char	*file_name;
	int		return_value;
	t_dlst	*temp_head;
	
	if (curr_node->content->outputs == NULL)
		return (SUCCESS);
	temp_head = curr_node->content->outputs;
	while (temp_head != NULL)
	{
		temp_head = temp_head->next;
		file_name = get_toklist_str(temp_head);
		if (get_toklist_type(temp_head->prev) == redir_output)
			fd = open(file_name, O_CREAT | O_TRUNC | O_WRONLY, 0644);
		else if (get_toklist_type(temp_head->prev) == append_output)
			fd = open(file_name, O_CREAT | O_APPEND | O_WRONLY, 0644);
		if (fd == FAILURE)
			return (print_exec_error(file_name, ERROR));
		return_value = dup2(fd, STDOUT_FILENO);
		if (close(fd) == FAILURE || return_value == FAILURE)
			return (return_perror("minishell: execution: redirection error", ERROR));
		temp_head = temp_head->next;
	}
	return (SUCCESS);
}

int	set_input(t_bin_tree *curr_node)
{
	int		fd;
	char	*file_name;
	int		return_value;
	t_dlst	*temp_head;
	
	if (curr_node->content->inputs == NULL)
		return (SUCCESS);
	temp_head = curr_node->content->inputs;
	while (temp_head != NULL)
	{
		temp_head = temp_head->next;
		file_name = get_toklist_str(temp_head);
		fd = open(file_name, O_RDONLY);
		if (fd == FAILURE)
			return (print_exec_error(file_name, ERROR));
		return_value = dup2(fd, STDIN_FILENO);
		if (close(fd) == FAILURE || return_value == FAILURE) //Faudra unlink si c'est un heredock
			return (return_perror("minishell: execution: redirection error", ERROR));
		temp_head = temp_head->next;
	}
	return (SUCCESS);
}