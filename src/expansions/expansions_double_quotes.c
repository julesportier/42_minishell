/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expansions_double_quotes.c                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: juportie <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/26 10:25:15 by juportie          #+#    #+#             */
/*   Updated: 2025/05/30 17:06:35 by juportie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../libft/src/libft.h"
#include "../minishell.h"
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
	char	*new_str,
	int		*i,
	t_shell_vars *shell_vars)
{
	int	var_id_len;
	char	*var_id;

	var_id_len = get_var_id_len(&quotes_content[*i+1]);
	if (var_id_len == 0)
	{
		if (quotes_content[*i+1] == '?')
		{
			new_str = join_char_free(new_str, shell_vars->last_cmd_ext_code + '0');
			++*i;
		}
		else
			new_str = join_char_free(new_str, quotes_content[*i]);
	}
	else
	{
		var_id = ft_substr(quotes_content, *i + 1, var_id_len);
		if (!var_id)
			return (NULL);
		new_str = free_strjoin(new_str, get_env_var_value(var_id, shell_vars->env), true, false);
		free (var_id);
		*i += var_id_len;
	}
	return (new_str);
}

t_dlst	*expand_double_quotes(
	t_dlst *token,
	t_shell_vars *shell_vars,
	t_error *error)
{
	char	*quotes_content;
	char	*new_str;
	int		i;

	quotes_content = get_toklist_str(token);
	if (quotes_content && quotes_content[0])
	{
		new_str = NULL;
		i = 0;
		while (quotes_content[i])
		{
			if (quotes_content[i] == '$' && quotes_content[i+1] != '\0')
				new_str = expand_quoted_variable(quotes_content, new_str, &i, shell_vars);
			else
				new_str = join_char_free(new_str, quotes_content[i]);
			if (!new_str)
			{
				*error = critical;
				return (NULL);
			}
			++i;
		}
		free(get_toklist_str(token));
		set_toklist_str(token, new_str);
	}
	set_toklist_type(token, literal);
	token = token->next;
	return (token);
}
