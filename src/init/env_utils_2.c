/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_utils_2.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: junsan <junsan@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/15 23:01:48 by junsan            #+#    #+#             */
/*   Updated: 2024/07/25 22:08:07 by junsan           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	insert_env_node(t_env **head, t_env *new_node)
{
	t_env	*cur;

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

void	add_env(t_env **head, const char *str)
{
	t_env	*new_node;
	char	*name;
	char	*content;

	name = NULL;
	content = NULL;
	env_split(str, &name, &content);
	if (name[0] == '_')
		new_node = new_env("_", "env");
	else
		new_node = new_env(name, content);
	(free(name), free(content));
	if (!name || !content || !new_node)
		return ;
	insert_env_node(head, new_node);
}

void	add_env_by_name(t_env *head, const char *name, const char *content)
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
