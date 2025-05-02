/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pwd.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ecasalin <ecasalin@42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/02 07:50:53 by ecasalin          #+#    #+#             */
/*   Updated: 2025/05/02 08:35:58 by ecasalin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <unistd.h>
#include <stdlib.h>
#include "../error_handling/errors.h"
#include "../general_utils/utils.h"
#include "../../libft/src/libft.h"

int	ms_pwd(void)
{
	char	*cwd;
	int		temp;

	cwd = getcwd(NULL, 0);
	if (cwd == NULL)
		return (return_perror("minishell: pwd critical error", CRIT_ERROR));
	cwd = free_strjoin(cwd, "\n");
	if (cwd == NULL)
		return (return_perror("minishell: pwd critical error", CRIT_ERROR));
	temp = write(1, cwd, ft_strlen(cwd));
	free(cwd);
	if (temp == FAILURE)
		return (return_perror("minishell: pwd", ERROR));
	return (SUCCESS);
}
