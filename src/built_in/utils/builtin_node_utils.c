/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_node_utils.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: junsan <junsan@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/12 10:32:58 by junsan            #+#    #+#             */
/*   Updated: 2024/08/06 10:52:17 by junsan           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_env	*builtin_new_node(char *name, char *content)
{
	t_env	*res;

	res = (t_env *)malloc(sizeof(t_env));
	if (!res)
		return (NULL);
	res->name = ft_strdup(name);
	res->content = NULL;
	if (content)
		res->content = ft_strdup(content);
	res->next = NULL;
	return (res);
}

static void	concatenate_content(char **dest, const char *src)
{
	size_t	new_size;
	char	*new_content;

	if (!src)
		return ;
	if (*dest)
		new_size = ft_strlen(*dest) + ft_strlen(src) + 1;
	else
		new_size = ft_strlen(src) + 1;
	new_content = (char *)malloc(new_size);
	if (!new_content)
	{
		perror("malloc error");
		return ;
	}
	if (*dest)
	{
		ft_strlcpy(new_content, *dest, new_size);
		ft_strlcat(new_content, src, new_size);
		free(*dest);
	}
	else
		ft_strlcpy(new_content, src, new_size);
	*dest = new_content;
}

void	append_builtin_node(t_env *head, char *name, char *content)
{
	t_env	*cur;

	if (!name || !head)
		return ;
	cur = head;
	while (cur)
	{
		if (ft_strncmp(cur->name, name, ft_strlen(name)) == 0)
		{
			concatenate_content(&cur->content, content);
			return ;
		}
		cur = cur->next;
	}
}

void	update_builtin_node(t_env *head, char *name, char *content)
{
	while (head)
	{
		if (ft_strncmp(name, head->name, ft_strlen(name)) == 0 && \
			ft_strlen(name) == ft_strlen(head->name))
		{
			if (head->content && content)
			{
				free(head->content);
				if (content)
					head->content = ft_strdup((char *)content);
			}
			return ;
		}
		head = head->next;
	}
}

void	add_builtin_node(t_env *head, char *name, char *content)
{
	t_env	*new_node;
	t_env	*cur;

	if (!name)
		return ;
	new_node = builtin_new_node(name, content);
	if (!new_node)
	{
		perror("malloc error");
		return ;
	}
	if (head == NULL)
		head = new_node;
	else
	{
		cur = head;
		while (cur->next)
			cur = cur->next;
		cur->next = new_node;
	}
}
