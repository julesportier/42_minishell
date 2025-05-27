/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ecasalin <ecasalin@42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/23 07:57:45 by ecasalin          #+#    #+#             */
/*   Updated: 2025/05/27 18:07:35 by ecasalin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>
#include <unistd.h>
#include "../parsing/parsing.h"
#include "../cleaning_utils/cleaning.h"
#include "../../libft/src/libft.h"
#include "builtins.h"
#include "../exec/exec.h"

static int	ft_isspace(char c)
{
	if ((c >= 9 && c <= 13) || c == 32)
		return (1);
	return (0);
}

static t_intf	atouc_flag(const char *nptr)
{
	int	i;
	int	sign;
	t_intf	nbr;

	nbr = (t_intf){0, 0};
	sign = 1;
	i = 0;
	while (ft_isspace(nptr[i]))
		i++;
	if (nptr[i] == '+' || nptr[i] == '-')
		if (nptr[i++] == '-')
			sign = -sign;
	if (!ft_isdigit(nptr[i]))
		nbr.flag = ERROR;
	while (nptr[i])
	{
		if (!ft_isdigit(nptr[i]))
			nbr.flag = ERROR;
		nbr.value = (nbr.value * 10) + (nptr[i] - 48);
		i++;
	}
	nbr.value *= sign;
	return (nbr);
}

int	ms_exit(char **args, t_shell_vars *vars, t_exit_error *exit_error)
{
	t_intf	exit_value;

	if (args[1] == NULL || args[1][0] == '\0')
	{
		printf("exit\n");
		return (vars->last_cmd_ext_code);
	}
	exit_value = atouc_flag(args[1]);
	if (exit_value.flag == ERROR)
		*exit_error = print_cmd_exec_issue("exit: ", args[1],
				": numeric argument required\n", not_a_digit);
	else if (args[2] != NULL)
		*exit_error = print_cmd_exec_issue("exit: ", NULL,
				"too many arguments\n", too_many_args);
	else
		printf("exit\n");
	if (*exit_error)
		vars->last_cmd_ext_code = *exit_error;
	else
		vars->last_cmd_ext_code = exit_value.value;
	return (vars->last_cmd_ext_code);
}
