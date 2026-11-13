/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_unset.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: junsan <junsan@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/29 14:48:24 by junsan            #+#    #+#             */
/*   Updated: 2024/08/19 17:26:59 by junsan           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_env	*ft_envlast(t_env *lst)
{
	if (!lst)
		return (0);
	while (lst->next)
		lst = lst->next;
	return (lst);
}

static void	remove_head_node(t_env *head)
{
	t_env	*next_node;

	free(head->name);
	if (head->content)
		free(head->content);
	next_node = head->next;
	if (next_node)
	{
		head->name = next_node->name;
		head->content = next_node->content;
		head->next = next_node->next;
		free(next_node);
	}
	else
	{
		head->name = ft_strdup("");
		head->content = NULL;
	}
}

int	del_node(t_env **lst, const char *to_unset)
{
	t_env	*cur;
	t_env	*prev;
	size_t	target_len;

	if (!lst || !*lst || !to_unset)
		return (0);
	cur = *lst;
	prev = NULL;
	target_len = ft_strlen(to_unset);
	while (cur)
	{
		if (ft_strlen(cur->name) == target_len && ft_strncmp(cur->name,
				to_unset, target_len) == 0)
		{
			if (prev == NULL)
				return (remove_head_node(cur), 1);
			prev->next = cur->next;
			(free(cur->name), free(cur->content), free(cur));
			return (1);
		}
		prev = cur;
		cur = cur->next;
	}
	return (0);
}

int	ft_unset(const char *cmd, const char **args, t_env *list)
{
	int i;

	(void)cmd;
	i = 1;
	if (!args || !list)
		return (FAILURE);
	while (args[i])
	{
		del_node(&list, args[i]);
		i++;
	}
	return (SUCCESS);
}