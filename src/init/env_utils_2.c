/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_utils_2.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: junsan <junsan@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/15 23:01:48 by junsan            #+#    #+#             */
/*   Updated: 2024/08/08 11:13:36 by junsan           ###   ########.fr       */
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
	char	*cur_dir;
	char	*loc_minishell;

	name = NULL;
	content = NULL;
	cur_dir = getcwd(NULL, 0);
	if (!cur_dir)
	{
		perror("getcwd error");
		return ;
	}
	loc_minishell = concat_two_strings(cur_dir, LOC_MINISHELL);
	env_split(str, &name, &content);
	if (name[0] == '_')
		new_node = new_env("_", loc_minishell);
	else
		new_node = new_env(name, content);
	(free(name), free(content), free(cur_dir), free(loc_minishell));
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
	char	*loc_minishell;

	cur_dir = getcwd(NULL, 0);
	if (!cur_dir)
		return (perror("getcwd error"), NULL);
	loc_minishell = concat_two_strings(cur_dir, LOC_MINISHELL);
	head->pwd = initialize_env_var("PWD", cur_dir);
	if (!head->pwd)
		return (perror("malloc error"), free(head), \
		free(cur_dir), free(loc_minishell), NULL);
	head->old_pwd = initialize_env_var("OLDPWD", NULL);
	if (!head->old_pwd)
		return (perror("malloc error"), \
		free(head), free(head->pwd), free(cur_dir), free(loc_minishell), NULL);
	head->last_arg = initialize_env_var("_", loc_minishell);
	if (!head->last_arg)
		return (perror("malloc error"), \
		free(head), free(head->pwd), free(head->old_pwd), \
		free(cur_dir), free(loc_minishell), NULL);
	return (free(cur_dir), free(loc_minishell), NULL);
}
