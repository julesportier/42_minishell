/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expansions_variable.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: juportie <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/26 10:25:15 by juportie          #+#    #+#             */
/*   Updated: 2025/05/30 16:18:56 by juportie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../libft/src/libft.h"
#include "../minishell.h"
#include "../general_utils/utils.h"
#include "../cleaning_utils/cleaning.h"
#include "expansions.h"

static t_dlst	*expand_value(t_dlst *token, char *value, t_error *error)
{
	char	**splits;
	int		i;

	splits = ft_split(value, ' ');
	if (!splits)
	{
		*error = critical;
		return (NULL);
	}
	i = 0;
	while (splits[i])
	{
		if (place_expansion_result(&token, variable, splits[i], error) != success)
			return (NULL);
		++i;
	}
	free_array(splits);
	token = token->next;
	return (token);
}

t_dlst	*expand_variable(
	t_dlst *token,
	t_dlst **toklist,
	t_shell_vars *shell_vars,
	t_error *error)
{
	char	*var_value;

	if (ft_isequalstr(get_toklist_str(token), "?"))
	{
		var_value = ft_itoa(shell_vars->last_cmd_ext_code);
		if (!var_value)
		{
			*error = critical;
			return (NULL);
		}
		token = expand_value(token, var_value, error);
		free(var_value);
	}
	else
	{
		var_value = get_env_var_value(get_toklist_str(token), shell_vars->env);
		if (!var_value || !var_value[0])
			remove_token(&token, toklist);
		else
			token = expand_value(token, var_value, error);
	}
	return (token);
}
