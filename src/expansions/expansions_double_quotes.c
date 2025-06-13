/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expansions_double_quotes.c                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: juportie <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/26 10:25:15 by juportie          #+#    #+#             */
/*   Updated: 2025/06/09 17:40:37 by juportie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../libft/src/libft.h"
#include "../minishell.h"
#include "../error_handling/errors.h"
#include "../general_utils/utils.h"
#include "expansions.h"

static int	get_var_id_len(char *var)
{
	int	i;

	i = 0;
	if (!ft_isalpha(var[i]) && var[i] != '_')
		return (i);
	++i;
	while (var[i] && var[i] != '=')
	{
		if (!ft_isalnum(var[i]) && var[i] != '_')
			break ;
		++i;
	}
	return (i);
}

static char	*expand_quoted_variable(
	char	*quotes_content,
	char	*str,
	int		*i,
	t_shell_vars *shell_vars)
{
	int		var_id_len;
	char	*var_id;

	var_id_len = get_var_id_len(&quotes_content[*i + 1]);
	if (var_id_len == 0)
	{
		if (quotes_content[*i + 1] == '?')
		{
			str = join_char_free(str, shell_vars->last_cmd_ext_code + '0', NULL);
			++*i;
		}
		else
			str = join_char_free(str, quotes_content[*i], NULL);
		if (!str)
			return (NULL);
	}
	else
	{
		var_id = ft_substr(quotes_content, *i + 1, var_id_len);
		if (!var_id)
			return (null_perror_alloc(critical, NULL));
		str = free_strjoin(
				str, get_env_var_value(var_id, shell_vars->env), true, false);
		free(var_id);
		if (!str)
			return (null_print_alloc(critical, NULL));
		*i += var_id_len;
	}
	return (str);
}

t_dlst	*expand_double_quotes(
	t_dlst *token,
	t_shell_vars *shell_vars,
	t_error *error)
{
	char	*inner_quotes;
	char	*str;
	int		i;

	inner_quotes = get_toklist_str(token);
	if (inner_quotes && inner_quotes[0])
	{
		str = NULL;
		i = 0;
		while (inner_quotes[i])
		{
			if (inner_quotes[i] == '$' && inner_quotes[i + 1] != '\0')
				str = expand_quoted_variable(inner_quotes, str, &i, shell_vars);
			else
				str = join_char_free(str, inner_quotes[i], error);
			if (!str)
				return (set_err_return_null(error, critical));
			++i;
		}
		free(get_toklist_str(token));
		set_toklist_str(token, str);
	}
	set_toklist_type(token, literal);
	token = token->next;
	return (token);
}
