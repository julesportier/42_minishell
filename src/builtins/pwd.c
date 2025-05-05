/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pwd.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ecasalin <ecasalin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/02 07:50:53 by ecasalin          #+#    #+#             */
/*   Updated: 2025/05/05 11:14:33 by ecasalin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <unistd.h>
#include <stdlib.h>
#include "../error_handling/errors.h"
#include "../general_utils/utils.h"
#include "../../libft/src/libft.h"


#include <stdio.h>

int	main(int ac, char *av[])
{
	char	*cwd;
	int		temp;

	cwd = getcwd(NULL, 0);
	if (cwd == NULL)
		return (return_perror("minishell: pwd critical error", CRIT_ERROR));
	// chdir(av[2]);
	// printf("%s\n", getcwd(NULL, 0));
	// exit(0);
	cwd = free_strjoin(cwd, "\n");
	if (cwd == NULL)
		return (return_perror("minishell: pwd critical error", CRIT_ERROR));
	temp = write(1, cwd, ft_strlen(cwd));
	free(cwd);
	if (temp == FAILURE)
		return (return_perror("minishell: pwd", ERROR));
	return (SUCCESS);
}
