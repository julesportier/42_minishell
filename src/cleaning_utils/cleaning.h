/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cleaning.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ecasalin <ecasalin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/25 17:59:52 by ecasalin          #+#    #+#             */
/*   Updated: 2025/05/26 13:42:08 by ecasalin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CLEANING_H
# define CLEANING_H

#include "../parsing/parsing.h"

void	free_array(char **array);
void	free_tree_and_vars(t_bin_tree *tree_root, t_shell_vars *vars);

#endif
