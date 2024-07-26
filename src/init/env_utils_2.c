/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_utils_2.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: junsan <junsan@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/15 23:01:48 by junsan            #+#    #+#             */
/*   Updated: 2024/07/26 22:51:29 by junsan           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	insert_env_node(t_env **head, t_env *new_node)
{
	t_env	*cur;

	if (*head == NULL)
	{
		init_pwd_oldpwd_under_score(new_node);
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

static t_env	*initialize_env_var(const char *name, const char *content)
{
	t_env	*new_env_var;

	new_env_var = (t_env *)malloc(sizeof(t_env));
	if (!new_env_var)
		return (perror("malloc error"), NULL);
	new_env_var->name = ft_strdup(name);
	new_env_var->content = NULL;
	if (content)
		new_env_var->content = ft_strdup((char *)content);
	new_env_var->next = NULL;
	return (new_env_var);
}

void	*init_pwd_oldpwd_under_score(t_env *head)
{
	char	*cur_dir;

	cur_dir = getcwd(NULL, 0);
	if (!cur_dir)
		return (perror("getcwd error"), NULL);
	head->pwd = initialize_env_var("PWD", cur_dir);
	if (!head->pwd)
		return (perror("malloc error"), free(head), free(cur_dir), NULL);
	head->old_pwd = initialize_env_var("OLDPWD", NULL);
	if (!head->old_pwd)
		return (perror("malloc error"), \
		free(head), free(head->pwd), free(cur_dir), NULL);
	head->last_arg = initialize_env_var("_", INIT_UNDER_SCORE);
	if (!head->last_arg)
		return (perror("malloc error"), \
		free(head), free(head->pwd), free(head->old_pwd), free(cur_dir), NULL);
	free(cur_dir);
	return (NULL);
}
