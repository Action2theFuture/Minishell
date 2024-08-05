/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   replace_env_vars_utils.c                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: junsan <junsan@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/26 16:53:48 by junsan            #+#    #+#             */
/*   Updated: 2024/08/05 12:43:54 by junsan           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static char	*get_env_content(char *arg, t_info *info)
{
	t_env	*env;

	env = info->env;
	while (env)
	{
		if ((ft_strncmp(env->name, arg, ft_strlen(arg)) == 0)
			&& (ft_strlen(arg) == ft_strlen(env->name)))
			return (env->content);
		env = env->next;
	}
	return (NULL);
}

char	*process_replace_env_vars(char *arg, t_info *info)
{
	char	*new_arg;
	char	*trim_str;
	char	*trim_space_in_each_str;

	new_arg = get_env_content(arg, info);
	if (new_arg == NULL)
	{
		new_arg = ft_strdup("");
		if (!new_arg)
			return (perror("malloc error"), NULL);
		return (new_arg);
	}
	trim_str = trim_whitespace(new_arg);
	trim_space_in_each_str = normalize_spaces(trim_str);
	if (trim_str)
		free(trim_str);
	return (trim_space_in_each_str);
}

char	*process_replace_expansion_var(t_info *info)
{
	char	*new_arg;
	int		status;

	new_arg = NULL;
	status = info->exit_status;
	if (status == 0)
		new_arg = ft_itoa(0);
	else if (status > 0)
		new_arg = ft_itoa(status);
	return (new_arg);
}

void	extract_var_name(\
			const char *str, size_t *i, char *var_name)
{
	size_t	var_start;

	var_start = *i;
	while (ft_isalnum(str[*i]) || str[*i] == '_')
		(*i)++;
	ft_strlcpy(var_name, str + var_start, *i - var_start + 1);
}

void	pass_double_quotes(t_env_var *env_var)
{
	if (env_var->str[env_var->i] == '\"' && env_var->is_value_expansion)
	{
		env_var->in_double_quotes = !env_var->in_double_quotes;
		env_var->i++;
	}
}
