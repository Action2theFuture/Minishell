/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handler_replace_env_vars_without_quotes.c          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: junsan <junsan@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/11 13:30:34 by junsan            #+#    #+#             */
/*   Updated: 2024/07/18 15:16:46 by junsan           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	handle_expansion_var(t_handler_info *h_info)
{
	char	*env_value;

	env_value = process_replace_expansion_var(h_info->info);
	if (env_value)
	{
		ft_strlcat(h_info->new_str, env_value, MAX_ARGS);
		h_info->new_str_len += ft_strlen(env_value);
		free(env_value);
	}
	h_info->i++;
}

static void	handle_normal_var(t_handler_info *h_info)
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

static void	extract_var_name_from_input(\
					t_handler_info *h_info, const char *input)
{
	h_info->var_len = 0;
	while (h_info->var_len < MAX_ARGS - 1 && \
		input[h_info->i] && !is_special_char(input[h_info->i]))
		h_info->var[h_info->var_len++] = input[h_info->i++];
	h_info->var[h_info->var_len] = '\0';
}

static bool	handle_quotes_from_input(t_handler_info *h_info, const char *input)
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

char	*handler_dollar_sign_wihout_quotes(\
		char *new_str, char *input, t_info *info)
{
	t_handler_info	h_info;

	init_handler_info(&h_info, new_str, info);
	h_info.new_str[0] = '\0';
	while (input[h_info.i])
	{
		if (input[h_info.i] == '$' && input[h_info.i + 1])
		{
			if (input[++h_info.i] == '?')
				handle_expansion_var(&h_info);
			else
			{
				extract_var_name_from_input(&h_info, input);
				handle_normal_var(&h_info);
			}
		}
		else
		{
			if (handle_quotes_from_input(&h_info, input))
				continue ;
			h_info.new_str[h_info.new_str_len++] = input[h_info.i++];
		}
	}
	return (h_info.new_str[h_info.new_str_len] = '\0', h_info.new_str);
}
