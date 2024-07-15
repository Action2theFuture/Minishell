/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_init.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: junsan <junsan@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/18 11:37:49 by junsan            #+#    #+#             */
/*   Updated: 2024/07/15 21:12:46 by junsan           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static t_env	*new_env(const char *name, const char *content)
{
	t_env	*new_node;

	new_node = (t_env *)malloc(sizeof(t_env));
	if (!new_node || !name || !content)
		return (NULL);
	new_node->name = ft_strdup(name);
	new_node->content = NULL;
	if (content)
		new_node->content = ft_strdup(content);
	new_node->next = NULL;
	return (new_node);
}

static void	*init_pwd_oldpwd(t_env *head)
{
	char	*cur_dir;

	if (!head)
		return (NULL);
	cur_dir = getcwd(NULL, 0);
	if (!cur_dir)
		return (perror("getcwd error"), NULL);
	head->pwd = (t_env *)malloc(sizeof(t_env));
	if (!head->pwd)
		return (perror("malloc error"), free(head), free(cur_dir), NULL);
	head->old_pwd = (t_env *)malloc(sizeof(t_env));
	if (!head->old_pwd)
		return (perror("malloc error"), \
		free(head), free(head->pwd), free(cur_dir), NULL);
	head->pwd->name = ft_strdup("PWD");
	head->pwd->content = cur_dir;
	head->pwd->next = NULL;
	head->old_pwd->name = ft_strdup("OLDPWD");
	head->old_pwd->content = NULL;
	head->old_pwd->next = NULL;
	return (NULL);
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

static void	add_env_minimum(t_env **head)
{
	char	*cur_dir;

	cur_dir = getcwd(NULL, 0);
	if (!cur_dir)
	{
		perror("getcwd error");
		return ;
	}
	*head = new_env("PWD", cur_dir);
	free(cur_dir);
	init_pwd_oldpwd(*head);
	(*head)->next = (t_env *)malloc(sizeof(t_env));
	if (!(*head)->next)
	{
		free(*head);
		return ;
	}
	(*head)->next->name = ft_strdup("OLDPWD");
	(*head)->next->content = NULL;
	(*head)->next->next = new_env("SHLVL", "1");
	(*head)->next->next->next = new_env("_", "/usr/bin/env");
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
