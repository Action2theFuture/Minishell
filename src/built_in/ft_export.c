/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_export.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ramzerk <ramzerk@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/04 23:38:01 by rabouzia          #+#    #+#             */
/*   Updated: 2024/06/28 14:22:25 by ramzerk          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	check_first_arg(const char arg)
{
	if (!ft_isalnum(arg) || arg != '_')
		return (1);
	else
		return (0);
}

int	ft_export(const char *cmd, const char **args, t_env *list)
{
	int		i;
	t_env	*tmp;
	char	*name;
	char	*content;

	(void)cmd;
	tmp = list;
	if (!args || !*args)
		return (0); // print sorted_env
	i = 0;
	while (args[i++])
	{
		if (check_first_arg(args[i][0]))
		{
			ft_putstr_fd("export: ", 1);
			ft_putstr_fd((char *)args[i], 1);
			ft_putstr_fd(": not a valid identifier\n", 1);
		}
		else
		{
			env_split(args[i], &name, &content);
			add_builtin_node(&tmp, name, content);
		}
	}
	return (0);
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

	head = env;
	if (env == NULL)
	{
		return (NULL);
	}
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
	env = sort_list(env);
	while (env)
	{
		printf("%s=%s\n", env->name, env->content);
		env = env->next;
	}
}