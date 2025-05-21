/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirections_utils.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ecasalin <ecasalin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/21 08:35:28 by ecasalin          #+#    #+#             */
/*   Updated: 2025/05/21 13:42:14 by ecasalin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include "../parsing/parsing.h"
#include "../error_handling/errors.h"
#include "exec.h"

int	set_input(t_bin_tree *curr_node)
{
	int		fd;
	char	*file_name;
	int		return_value;
	t_dlst	*temp_head;
	
	if (curr_node->content->inputs == NULL)
		return (SUCCESS);
	while (curr_node->content->inputs != NULL)
	{
		temp_head = curr_node->content->inputs->next;
		file_name = get_toklist_str(temp_head);
		fd = open(file_name, O_RDONLY);
		if (fd == FAILURE)
			return (print_exec_error(file_name, ERROR));
		return_value = dup2(fd, STDIN_FILENO);
		if (close(fd) == FAILURE || return_value == FAILURE)
			return (return_perror("minishell: execution: redirection error", ERROR));
		temp_head = temp_head->next->next;
	}
	return (SUCCESS);
}