/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export_errors_utils.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ecasalin <ecasalin@42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/01 19:04:53 by ecasalin          #+#    #+#             */
/*   Updated: 2025/05/01 19:13:10 by ecasalin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <unistd.h>
#include "../../libft/src/libft.h"
#include "../general_utils/utils.h"
#include "../minishell.h"

int	write_var_name_error(char *var)
{
	int		var_name_len;
	char	*var_name;
	char	*error_msg;

	var_name_len = get_var_name_len(var);
	var_name = ft_calloc(var_name_len + 1, 1);
	error_msg = ft_strdup("minishell: export: `");
	if (var_name == NULL || error_msg == NULL)
		return (CRIT_ERROR);
	ft_memcpy(var_name, var, var_name_len);
	error_msg = free_strjoin(error_msg, var_name);
	free(var_name);
	if (error_msg == NULL)
		return (CRIT_ERROR);
	error_msg = free_strjoin(error_msg, "': not a valid identifier\n");
	if (error_msg == NULL)
		return (CRIT_ERROR);
	write(2, error_msg, ft_strlen(error_msg));
	free(error_msg);
	return (ERROR);
}
