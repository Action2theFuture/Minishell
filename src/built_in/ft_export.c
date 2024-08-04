/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_export.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: junsan <junsan@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/04 23:38:01 by junsan            #+#    #+#             */
/*   Updated: 2024/08/04 13:36:26 by junsan           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	ft_export_show(t_env *env)
{
	t_env	*cur;

	cur = env;
	while (cur)
	{
		if (!cur->content)
			printf("export %s\n", cur->name);
		else
			printf("export %s=%s\n", cur->name, cur->content);
		cur = cur->next;
	}
}

static int	handle_invalid_identifier(const char *arg)
{
	ft_putstr_fd("export: ", 2);
	ft_putstr_fd((char *)arg, 2);
	ft_putstr_fd(": not a valid identifier\n", 2);
	return (FAILURE);
}

static void	split_name_content(\
		const char *arg, char **name, char **content, bool *is_plus)
{
	if (ft_strchr(arg, '='))
	{
		env_split(arg, name, content);
		if (strip_trailing_plus(*name))
			*is_plus = true;
	}
	else
		*name = ft_strdup((char *)arg);
}

static void	process_export_arg(\
		char *name, char *content, bool is_plus, t_env *list)
{
	bool	is_var_in_env;

	is_var_in_env = var_if_exist(name, list);
	if (is_var_in_env)
	{
		if (is_plus)
			append_builtin_node(list, name, content);
		else
			update_builtin_node(list, name, content);
	}
	else if (!is_var_in_env || is_plus)
		add_builtin_node(list, name, content);
}

int	ft_export(const char *cmd, const char **args, t_env *list)
{
	int		i;
	int		status;
	char	*name;
	char	*content;
	bool	is_plus;

	((void)cmd, status = SUCCESS);
	if (!args[1])
		return (ft_export_show(list), SUCCESS);
	i = 0;
	while (args[++i])
	{
		content = NULL;
		name = NULL;
		is_plus = false;
		split_name_content(args[i], &name, &content, &is_plus);
		if (is_valid_name(name))
			process_export_arg(name, content, is_plus, list);
		else
			status = handle_invalid_identifier(args[i]);
		free(name);
		if (content)
			free(content);
	}
	return (status);
}
