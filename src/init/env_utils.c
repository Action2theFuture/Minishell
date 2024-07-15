/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_utils.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: junsan <junsan@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/23 18:04:20 by junsan            #+#    #+#             */
/*   Updated: 2024/06/23 18:04:32 by junsan           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	clear_env(t_env *head)
{
	t_env	*cur;

	if (head == NULL)
		return ;
	while (head)
	{
		cur = head->next;
		free(head->name);
		free(head->content);
		free(head);
		head = cur;
	}
	head = NULL;
}

size_t	env_size(t_env *head)
{
	t_env	*cur;
	size_t	size;

	size = 0;
	cur = head;
	while (cur)
	{
		size++;
		cur = cur->next;
	}
	return (size);
}
