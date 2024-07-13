/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_pwd_oldpwd.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: junsan <junsan@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/12 22:55:56 by junsan            #+#    #+#             */
/*   Updated: 2024/07/13 11:31:26 by junsan           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	swap_pwd_oldpwd(t_env *env)
{
	char	*tmp;

	tmp = env->pwd->content;
	env->pwd->content = env->old_pwd->content;
	env->old_pwd->content = tmp;
}

void	update_pwd_oldpwd(t_env *env, const char *new_pwd)
{
	t_env	*head;

	head = env;
	if (head->old_pwd->content)
		free(head->old_pwd->content);
	head->old_pwd->content = head->pwd->content;
	head->pwd->content = ft_strdup(new_pwd);
	while (env)
	{
		if (ft_strncmp(env->name, "OLDPWD", 6) == 0)
		{
			free(env->content);
			env->content = ft_strdup(head->old_pwd->content);
		}
		else if (ft_strncmp(env->name, "PWD", 3) == 0)
		{
			free(env->content);
			env->content = ft_strdup(head->pwd->content);
		}
		env = env->next;
	}
}

void	init_pwd_oldpwd_nodes(t_env *head, t_env *old_pwd, t_env *pwd)
{
	if (old_pwd)
		add_pwd_oldpwd(head, old_pwd->name, old_pwd->content);
	else
		add_pwd_oldpwd(head, "OLDPWD", NULL);
	if (pwd)
		add_pwd_oldpwd(head, pwd->name, pwd->content);
	else
		add_pwd_oldpwd(head, "PWD", NULL);
}

int	init_oldpwd_node(t_env *lst)
{
	t_env	*head;
	t_env	*old_pwd;
	t_env	*pwd;

	head = lst;
	if (!head)
		return (0);
	old_pwd = NULL;
	pwd = NULL;
	while (lst)
	{
		if (ft_strncmp(lst->name, "OLDPWD", 6) == 0)
			old_pwd = lst;
		else if (ft_strncmp(lst->name, "PWD", 3) == 0)
			pwd = lst;
		lst = lst->next;
	}
	init_pwd_oldpwd_nodes(head, pwd, old_pwd);
	return (0);
}

void	add_pwd_oldpwd(t_env *head, const char *name, const char *content)
{
	if (ft_strncmp(name, "OLDPWD", 6) == 0)
	{
		if (content)
		{
			free(head->old_pwd->content);
			head->old_pwd->content = ft_strdup(content);
		}
	}
	else if (ft_strncmp(name, "PWD", 3) == 0)
	{
		if (content)
		{
			free(head->pwd->content);
			head->pwd->content = ft_strdup(content);
		}
	}
}
