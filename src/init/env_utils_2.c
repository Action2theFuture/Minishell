/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_utils_2.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: junsan <junsan@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/15 23:01:48 by junsan            #+#    #+#             */
/*   Updated: 2024/07/16 00:11:31 by junsan           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	add_env(t_env **head, const char *str)
{
	t_env	*new_node;
	t_env	*cur;
	char	*name;
	char	*content;

	name = NULL;
	content = NULL;
	env_split(str, &name, &content);
	new_node = new_env(name, content);
	free(name);
	free(content);
	if (!name || !content || !new_node)
		return ;
	if (*head == NULL)
	{
		init_pwd_oldpwd(new_node);
		*head = new_node;
	}
	else
	{
		cur = *head;
		while (cur->next)
			cur = cur->next;
		cur->next = new_node;
	}
}

void	add_env_node(t_env *head, const char *name, const char *content)
{
	t_env	*new_node;
	t_env	*cur;

	new_node = new_env(name, content);
	if (!head)
	{
		head = new_node;
		return ;
	}
	else
	{
		cur = head;
		while (cur->next)
			cur = cur->next;
		cur->next = new_node;
	}
}
