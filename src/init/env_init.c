/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_init.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: junsan <junsan@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/18 11:37:49 by junsan            #+#    #+#             */
/*   Updated: 2024/08/05 13:58:18 by junsan           ###   ########.fr       */
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
		add_env_by_name(*head, "PWD", cur_dir);
		free(cur_dir);
	}
	if (!is_check_key("OLDPWD", cur))
		add_env_by_name(*head, "OLDPWD", NULL);
	if (!is_check_key("SHLVL", cur))
		add_env_by_name(*head, "SHLVL", "1");
	if (!is_check_key("_", cur))
		add_env_by_name(*head, "_", INIT_UNDER_SCORE);
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
	init_pwd_oldpwd_under_score(*head);
	add_env_by_name(*head, "OLDPWD", NULL);
	add_env_by_name(*head, "SHLVL", "1");
	add_env_by_name(*head, "_", INIT_UNDER_SCORE);
	free(cur_dir);
}

t_env	*env_init(char **envp)
{
	t_env	*env;
	int		i;

	env = NULL;
	i = -1;
	while (envp[++i])
		add_env(&env, envp[i]);
	if (env == NULL)
		initialize_default_env(&env);
	add_env_minimum_required_env(&env);
	return (env);
}
