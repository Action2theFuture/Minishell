/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing_utils_2.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: junsan <junsan@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/27 15:40:23 by junsan            #+#    #+#             */
/*   Updated: 2024/07/19 10:11:47 by junsan           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	add_parent(t_ast *node, t_ast *left, t_ast *right)
{
	if (left)
		left->parent = node;
	if (right)
		right->parent = node;
}
