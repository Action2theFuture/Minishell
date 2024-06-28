/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_unset.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ramzerk <ramzerk@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/29 14:48:24 by junsan            #+#    #+#             */
/*   Updated: 2024/06/28 14:23:00 by ramzerk          ###   ########.fr       */
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

int	del_node(t_env *lst, const char *to_unset)
{
	t_env	*cur;
	t_env	*prev;
	t_env	*next;

	// gerer le cas ou on enleve le premier notamment en mettant **lst
	// si le noeud nexiste pas ou il ny a pas dargument aucun msg derreur
	cur = lst;
	prev = cur;
	while (cur)
	{
		if (cur->name == to_unset)
		{
			if (cur->next != NULL)
				next = cur->next;
			break ;
		}
		prev = cur;
		cur = cur->next;
	}
	free(cur);
	if (ft_envlast(lst) == prev)
		prev->next = NULL;
	else
		prev->next = next;
	return (1);
}

int	ft_unset(const char *cmd, const char **args, t_env *list)
{
	t_env	*cur;
	int		i;

	cur = list;
	(void)cmd;
	i = 0;
	if (!args)
		return (0);
	while (args[i])
	{
		del_node(cur, args[i]);
		i++;
	}
	return (0);
}
