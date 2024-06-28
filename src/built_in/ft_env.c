/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_env.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ramzerk <ramzerk@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/29 11:48:50 by junsan            #+#    #+#             */
/*   Updated: 2024/06/28 14:26:30 by ramzerk          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	printf_env(t_env *list)
{
	t_env	*cur;

	cur = list;
	while (cur)
	{
		printf("%s=%s\n", cur->name, cur->content);
		cur = cur->next;
	}
}
static int	check_in_env(const char *arg, t_env *list)
{
	t_env	*cur;

	cur = list;
	while (cur)
	{
		if (ft_strncmp(arg, cur->name, ft_strlen(cur->name)) == 0)
		{
			ft_putstr_fd(cur->content, 1);
			printf_env(list);
			return (0);
		}
		cur = cur->next;
	}
	return (0);
}

// UnCompleted
int	ft_env(const char *cmd, const char **args, t_env *list)
{
	(void)cmd;
	if (!args)
	{
		printf_env(list);
		return (0);
	}
	if (args[0])
	{
		if (sizeof(args) > 2)
		{
			ft_putstr_fd("ignoring non-option arguments", STDOUT_FILENO);
			printf_env(list);
		}
		else
			return (check_in_env(args[0], list));
	}
	return (0);
}

t_env	*builtin_new_node(char *name, char *content)
{
	t_env	*res;

	res = (t_env *)malloc(sizeof(t_env));
	if (!res)
		return (NULL);
	res->name = ft_strdup(name);
	res->content = ft_strdup(content);
	res->next = NULL;
	return (res);
}

void	add_builtin_node(t_env **head, char *name, char *content)
{
	t_env	*new_node;
	t_env	*cur;

	new_node = builtin_new_node(name, content);
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

void	env_split(const char *str, char **name, char **content)
{
	size_t	i;
	size_t	j;

	if (!str)
		return ;
	i = 0;
	j = 1;
	// tmp = 0;
	while (str[i] != '=')
		i++;
	*name = ft_strndup(str, i);
	i++;
	j = i;
	while (str[i])
		i++;
	*content = ft_strndup(str, i - j);
}

t_env	*fill_env(int ac, char **av, char **env)
{
	char	*name;
	char	*content;
	t_env	*arg;
	t_env	*tmp;
	int		i;

	(void)ac;
	(void)av;
	arg = ft_calloc(1, sizeof(t_env));
	if (!arg)
		return (NULL);
	i = -1;
	name = NULL;
	content = NULL;
	tmp = arg;
	while (env[++i])
	{
		env_split(env[i], &name, &content);
		add_builtin_node(&tmp, name, content);
		free(name);
		free(content);
	}
	arg = tmp;
	return (arg);
}
