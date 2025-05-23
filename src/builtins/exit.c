/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ecasalin <ecasalin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/23 07:57:45 by ecasalin          #+#    #+#             */
/*   Updated: 2025/05/23 09:48:35 by ecasalin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>
#include <unistd.h>
#include "../parsing/parsing.h"
#include "../cleaning_utils/cleaning.h"
#include "../../libft/src/libft.h"
#include "builtins.h"
#include "../exec/exec.h"

static void	clean_and_exit(char **args, t_bin_tree *curr_node, t_shell_vars *vars, int exit_value)
{
	free_array(args);
	free_tree_and_vars(tree_root(curr_node), vars);
	printf("exit\n");
	exit(exit_value);
}

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
	{
		if (nptr[i] == '-')
			sign = -sign;
		i++;
	}
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

int	ms_exit(char **args, t_bin_tree *curr_node, t_shell_vars *vars)
{
	int		i;
	t_intf	exit_value;
	
	i = 0;
	exit_value.value = vars->last_cmd_ext_code;
	if (args[1] == NULL || args[1][0] == '\0')
		clean_and_exit(args, curr_node, vars, exit_value.value);
	exit_value = atouc_flag(args[1]);
	if (exit_value.flag == ERROR)
		return (print_cmd_exec_issue("exit: ", args[1], ": numeric argument required\n", 2));
	if (args[2] != NULL)
		return (print_cmd_exec_issue("exit: ", NULL, "too many arguments\n", 1));
	clean_and_exit(args, curr_node, vars, exit_value.value);
	return (SUCCESS);
}
