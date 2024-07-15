/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_init.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: junsan <junsan@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/18 11:37:49 by junsan            #+#    #+#             */
/*   Updated: 2024/07/16 00:11:47 by junsan           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_env	*new_env(const char *name, const char *content)
{
	t_env	*new_node;

	new_node = (t_env *)malloc(sizeof(t_env));
	if (!new_node || !name)
		return (NULL);
	new_node->name = ft_strdup(name);
	new_node->content = NULL;
	if (content)
		new_node->content = ft_strdup(content);
	new_node->next = NULL;
	return (new_node);
}

void	*init_pwd_oldpwd(t_env *head)
{
	char	*cur_dir;

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

static void	add_env_minimum_required_env(t_env **head)
{
	char	*cur_dir;
	t_env	*cur;

	cur = *head;
	if (!is_check_key("PWD", cur))
	{
		cur_dir = getcwd(NULL, 0);
		if (!cur_dir)
		{
			perror("getcwd error");
			return ;
		}
		add_env_node(*head, "PWD", cur_dir);
		free(cur_dir);
	}
	if (!is_check_key("OLDPWD", cur))
		add_env_node(*head, "OLDPWD", NULL);
	if (!is_check_key("SHLVL", cur))
		add_env_node(*head, "SHLVL", "1");
	if (!is_check_key("_", cur))
		add_env_node(*head, "_", "/usr/bin/env");
}

static void	initialize_default_env(t_env **head)
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
	add_env_node(*head, "OLDPWD", NULL);
	add_env_node(*head, "SHLVL", "1");
	add_env_node(*head, "_", "/usr/bin/env");
}

t_env	*env_init(char **envp)
{
	t_env	*env;
	int		i;

	env = NULL;
	i = -1;
	while (envp[++i])
		add_env(&env, envp[i]);
	add_env_minimum_required_env(&env);
	if (env == NULL)
		initialize_default_env(&env);
	return (env);
}
