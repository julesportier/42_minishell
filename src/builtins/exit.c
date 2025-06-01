/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ecasalin <ecasalin@42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/23 07:57:45 by ecasalin          #+#    #+#             */
/*   Updated: 2025/06/01 10:54:18 by ecasalin         ###   ########.fr       */
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

static int	set_numeric_arg_error(t_error *error, char *arg)
{
	*error = invalid_exit_args;
	print_joined_cmd_error("exit: ", arg,
			": numeric argument required\n", error);
	 return (2);
}

static int	set_too_many_args_error(t_error *error)
{
	*error = invalid_exit_args;
	print_joined_cmd_error("exit: ", NULL,
			"too many arguments\n", error);
	return (1);
}

int	ms_exit(char **args, t_shell_vars *vars, t_error *error)
{
	t_intf	arg_value;
	int		return_value;

	if (args[1] == NULL || args[1][0] == '\0')
	{
		printf("exit\n");
		return (vars->last_cmd_ext_code);
	}
	arg_value = atouc_flag(args[1]);
	return_value = arg_value.value;
	if (arg_value.flag == ERROR)
		return_value = set_numeric_arg_error(error, args[1]);
	else if (args[2] != NULL)
		return_value = set_too_many_args_error(error);
	else
		printf("exit\n");
	return (return_value);
}
