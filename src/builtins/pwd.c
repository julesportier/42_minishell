/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pwd.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ecasalin <ecasalin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/02 07:50:53 by ecasalin          #+#    #+#             */
/*   Updated: 2025/05/15 11:35:21 by ecasalin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
#include "../error_handling/errors.h"
#include "../general_utils/utils.h"
#include "../../libft/src/libft.h"

int	ms_pwd(void)
{
	char	*cwd;
	int		temp;

	cwd = getcwd(NULL, 0);
	if (cwd == NULL && errno == ENOMEM)
		return (return_perror("minishell: pwd critical error", CRIT_ERROR));
	if (cwd == NULL)
		return (return_perror("minishell: pwd: getcwd error", ERROR));
	temp = printf("%s\n", cwd);
	free(cwd);
	if (temp < 0)
		return (return_perror("minishell: pwd", ERROR));
	return (SUCCESS);
}
