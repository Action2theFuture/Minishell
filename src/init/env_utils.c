/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_utils.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: junsan <junsan@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/23 18:04:20 by junsan            #+#    #+#             */
/*   Updated: 2024/07/12 22:55:11 by junsan           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	clear_pwd_oldpwd(t_env *head)
{
	if (head->pwd)
	{
		free(head->pwd->name);
		if (head->pwd->content)
			free(head->pwd->content);
		free(head->pwd);
	}
	if (head->old_pwd)
	{
		free(head->old_pwd->name);
		if (head->old_pwd->content)
			free(head->old_pwd->content);
		free(head->old_pwd);
	}
}

void	clear_env(t_env *head)
{
	t_env	*cur;

	if (head == NULL)
		return ;
	clear_pwd_oldpwd(head);
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
