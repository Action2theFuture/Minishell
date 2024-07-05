/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_init.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ramzerk <ramzerk@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/18 11:37:49 by junsan            #+#    #+#             */
/*   Updated: 2024/07/04 16:23:28 by junsan           ###   ########.fr       */
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

static t_env	*new_env(const char *name, const char *content)
{
	t_env	*new_node;

	new_node = (t_env *)malloc(sizeof(t_env));
	if (!new_node || !name || !content)
		return (NULL);
	new_node->name = ft_strdup(name);
	new_node->content = ft_strdup(content);
	new_node->next = NULL;
	return (new_node);
}

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
		*head = new_node;
	else
	{
		cur = *head;
		while (cur->next)
			cur = cur->next;
		cur->next = new_node;
	}
}

static void	add_env_minimum(t_env **head)
{
	char	*cur_dir;

	cur_dir = getcwd(NULL, 0);
	if (!cur_dir)
	{
		perror("getcwd error");
		return ;
	}
	(*head) = new_env("PWD", cur_dir);
	(*head)->next = new_env("SHLVL", "1");
	(*head)->next->next = new_env("_", "/usr/bin/env");
	free(cur_dir);
}

t_env	*env_init(char **envp)
{
	t_env	*env;
	int		i;

	i = -1;
	env = NULL;
	while (envp[++i])
		add_env(&env, envp[i]);
	if (env == NULL)
		add_env_minimum(&env);
	return (env);
}
