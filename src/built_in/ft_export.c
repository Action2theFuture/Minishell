/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_export.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: junsan <junsan@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/04 23:38:01 by rabouzia          #+#    #+#             */
/*   Updated: 2024/07/12 10:55:57 by junsan           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	ft_export_show(t_env *env)
{
	t_env	*cur;

	cur = find_lowest(env);
	while (cur)
	{
		if (!cur->content)
			printf("export %s\n", cur->name);
		else
			printf("export %s=%s\n", cur->name, cur->content);
		cur = find_next_lower(env, cur->name);
	}
}

static void	handle_invalid_identifier(const char *arg)
{
	ft_putstr_fd("export: ", 2);
	ft_putstr_fd((char *)arg, 2);
	ft_putstr_fd(": not a valid identifier\n", 2);
}

int	ft_export(const char *cmd, const char **args, t_env *list)
{
	int		i;
	t_env	*tmp;
	char	*name;
	char	*content;

	(void)cmd;
	content = NULL;
	name = NULL;
	tmp = list;
	if (!args[1] || !*args)
		return (ft_export_show(list), FAILURE);
	i = 0;
	while (args[++i])
	{
		env_split(args[i], &name, &content);
		if (!change_var_if_exist(name, content, list))
		{
			if (check_first_arg(args[i][0]))
				handle_invalid_identifier(args[i]);
			else
				add_builtin_node(&tmp, name, content);
		}
	}
	return (SUCCESS);
}
