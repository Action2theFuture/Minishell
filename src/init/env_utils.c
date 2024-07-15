/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_utils.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: junsan <junsan@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/23 18:04:20 by junsan            #+#    #+#             */
/*   Updated: 2024/07/15 20:26:04 by junsan           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	env_split(const char *str, char **name, char **content)
{
	size_t	i;

	if (!str)
		return ;
	i = 0;
	while (str[i] && str[i] != '=')
		i++;
	if (str[i] != '=')
	{
		*name = (char *)str;
		*content = NULL;
		return ;
	}
	*name = ft_strndup(str, i);
	*content = ft_strndup(str + i + 1, ft_strlen(str) - i - 1);
	if (*content == NULL)
	{
		free(*name);
		*name = NULL;
	}
}

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
