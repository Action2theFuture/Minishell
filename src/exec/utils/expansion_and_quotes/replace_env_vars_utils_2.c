/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   replace_env_vars_utils_2.c                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: junsan <junsan@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/26 21:57:01 by junsan            #+#    #+#             */
/*   Updated: 2024/07/26 22:40:26 by junsan           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	handle_expansion_var_without_quotes(t_handler_info *h_info, char c)
{
	char	*env_value;

	if (c == '?')
	{
		env_value = process_replace_expansion_var(h_info->info);
		if (env_value)
		{
			ft_strlcat(h_info->new_str, env_value, MAX_ARGS);
			h_info->new_str_len += ft_strlen(env_value);
			free(env_value);
		}
	}
	else if (c == '_')
	{
		env_value = NULL;
		if (!h_info->info->env->last_arg->content)
			env_value = ft_strdup(INIT_UNDER_SCORE);
		else
			env_value = ft_strdup(h_info->info->env->last_arg->content);
		ft_strlcat(h_info->new_str, env_value, MAX_ARGS);
		h_info->new_str_len += ft_strlen(env_value);
		free(env_value);
	}
	h_info->i++;
}

void	handle_normal_var_without_quotes(t_handler_info *h_info)
{
	char	*env_value;

	env_value = process_replace_env_vars(h_info->var, h_info->info);
	if (env_value)
	{
		ft_strlcat(h_info->new_str, env_value, MAX_ARGS);
		h_info->new_str_len += ft_strlen(env_value);
		free(env_value);
	}
}

void	extract_var_name_from_input(\
					t_handler_info *h_info, const char *input)
{
	h_info->var_len = 0;
	while (h_info->var_len < MAX_ARGS - 1 && \
		input[h_info->i] && !is_special_char(input[h_info->i]))
		h_info->var[h_info->var_len++] = input[h_info->i++];
	h_info->var[h_info->var_len] = '\0';
}

bool	handle_quotes_from_input(t_handler_info *h_info, const char *input)
{
	if (input[h_info->i] == '\'' && !(h_info->in_double_quotes))
	{
		h_info->in_single_quotes = !(h_info->in_single_quotes);
		h_info->i++;
		return (true);
	}
	else if (input[h_info->i] == '"' && !(h_info->in_single_quotes))
	{
		h_info->in_double_quotes = !(h_info->in_double_quotes);
		h_info->i++;
		return (true);
	}
	return (false);
}
