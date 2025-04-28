/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cleaning.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kura <kura@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/25 17:59:52 by kura              #+#    #+#             */
/*   Updated: 2025/04/25 18:30:54 by kura             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CLEANING_H
# define CLEANING_H

void	*free_array(char **array);
int		close_fd(int *fd);
int		close_pipe(int *pipe);
int		close_pipe_perror(char *err_msg, int return_value, int *pipe);

#endif
