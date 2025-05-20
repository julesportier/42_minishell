/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   subprocess_utils.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ecasalin <ecasalin@42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/21 14:20:47 by ecasalin          #+#    #+#             */
/*   Updated: 2025/05/20 21:24:05 by ecasalin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <unistd.h>
#include <sys/wait.h>
#include "exec.h"

int	get_exit_code(int child_exit_status)
{
	if (!WIFEXITED(child_exit_status))
		return (128 + WTERMSIG(child_exit_status));
	return (WEXITSTATUS(child_exit_status));
}

int	wait_childs(pid_t second_child_pid)
{
	int	child_exit_status;

	waitpid(second_child_pid, &child_exit_status, 0);
	waitpid(0, NULL, 0);
	return (get_exit_code(child_exit_status));
}
