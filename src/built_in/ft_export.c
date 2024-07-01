/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_export.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ramzerk <ramzerk@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/04 23:38:01 by rabouzia          #+#    #+#             */
/*   Updated: 2024/07/01 16:32:23 by ramzerk          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	check_first_arg(const char arg)
{
	if (ft_isalpha(arg) || arg == '_')
		return (0);
	else
		return (1);
}

t_env	*var_find_if_exist(const char *var, t_env *lst)
{
	while (lst)
	{
		if (ft_strncmp(var, lst->name, ft_strlen(var)) == 0 )
			return (lst);
		lst = lst->next;
	}
	return (NULL);
}

int	ft_strcmp(char *s1, char *s2)
{
	int i;

	i = 0;
	while (s1[i] == s2[i] && s1[i] != '\0' && s2[i] != '\0')
		i++;
	return (s1[i] - s2[i]);
}

t_env *find_next_lower(t_env *lst, char *name)
{
    t_env *cur = lst;
    t_env *next_lower = NULL;

    while (cur)
    {
        if (strcmp(cur->name, name) < 0)
        {
            if (!next_lower || strcmp(cur->name, next_lower->name) > 0)
                next_lower = cur;
        }
        cur = cur->next;
    }
    return next_lower;
}

t_env *find_lowest(t_env *lst)
{
    t_env *cur = lst;
    t_env *lowest = cur;

    while (cur)
    {
        if (strcmp(cur->name, lowest->name) < 0)
            lowest = cur;
        cur = cur->next;
    }
    return lowest;
}

void ft_export_show(t_env *env)
{
    t_env *cur = env;
    t_env *next_lower= find_lowest(env);

    while (cur)
    {
        if (!cur->content)
            printf("export %s\n", cur->name);
        else
            printf("export %s=%s\n", cur->name, cur->content);
        next_lower = find_next_lower(env, cur->name);
    	cur = next_lower;
    }
}

// modifier une variable si elle existe deja
// expand

int	ft_export(const char *cmd, const char **args, t_env *list)
{
	int		i;
	t_env	*tmp;
	t_env	*tmp2;
	char	*name;
	char	*content;

	content = name = NULL;
	(void)cmd;
	tmp = list;
	tmp2 = list;
	if (!args[1] || !*args)
		return (ft_export_show(list), 1);
	i = 1;
	while (args[i])
	{
		env_split(args[i], &name, &content);
		tmp2 = var_find_if_exist(name, tmp2);
		if (tmp2)
		{
			free(tmp2->content);
			tmp2->content = content;
		}
		else 
		{	
			if (check_first_arg(args[i][0]))
			{
				ft_putstr_fd("export: ", 2);
				ft_putstr_fd((char *)args[i], 2);
				ft_putstr_fd(": not a valid identifier\n", 2);
			}
			else
				add_builtin_node(&tmp, name, content);
		}
		i++;
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
	if (!name || !new_node)
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
