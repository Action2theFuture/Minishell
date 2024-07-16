/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_utils_for_expansion_2.c                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: junsan <junsan@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/16 13:35:09 by junsan            #+#    #+#             */
/*   Updated: 2024/07/16 14:04:09 by junsan           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	clear_expand_info(t_expand_info *head)
{
	t_expand_info	*cur;

	if (!head)
		return ;
	while (head)
	{
		cur = head->next;
		if (head->matches)
			free_args(head->matches);
		free(head);
		head = cur;
	}
	head = NULL;
}
