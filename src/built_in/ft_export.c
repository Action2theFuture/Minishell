/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_export.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rabouzia <rabouzia@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/04 23:38:01 by rabouzia          #+#    #+#             */
/*   Updated: 2024/06/29 20:36:56 by rabouzia         ###   ########.fr       */
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

void	swap_nodes(t_env *a, t_env *b)
{
	char	*tmp_name;
	char	*tmp_content;

	tmp_name = a->name;
	tmp_content = a->content;
	a->name = b->name;
	a->content = b->content;
	b->name = tmp_name;
	b->content = tmp_content;
}

t_env	*sort_list(t_env *env)
{
	t_env	*head;
	t_env	*current;
	int		swapped;

	head = ft_calloc(sizeof(t_env),1);
	head = env;
	if (env == NULL)
		return (NULL);
	swapped = 1;
	while (swapped)
	{
		swapped = 0;
		current = head;
		while (current->next != NULL)
		{
			if (ft_strncmp(current->name, current->next->name,ft_strlen(current->name)) > 0)
			{
				swap_nodes(current, current->next);
				swapped = 1;
			}
			current = current->next;
		}
	}
	return (head);
}

void	ft_export_show(t_env *env)
{
	t_env *tmp;
	
	tmp = sort_list(env);
	while (tmp)
	{
		printf("%s=%s\n", tmp->name, tmp->content);
		tmp = tmp->next;
	}
}

// modifier une variable si elle existe deja
// expand 


int	ft_export(const char *cmd, const char **args, t_env *list)
{
	int		i;
	t_env	*tmp;
	char	*name;
	char	*content;

	(void)cmd;
	tmp = list;
	if (!args[1] || !*args)
		return (ft_export_show(list), 1); // print sorted_env
	i = 1;
	while (args[i])
	{
		// printf("check_first %d\n",check_first_arg(args[i][0]));
		if (check_first_arg(args[i][0]))
		{
	
			ft_putstr_fd("export: ", 2);
			ft_putstr_fd((char *)args[i], 2);
			ft_putstr_fd(": not a valid identifier\n", 2);
		}
		else
		{
			env_split(args[i], &name, &content);
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




