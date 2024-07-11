/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handler_replace_env_vars_without_quotes.c          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: junsan <junsan@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/11 13:30:34 by junsan            #+#    #+#             */
/*   Updated: 2024/07/11 17:34:35 by junsan           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static bool	is_special_char(char c)
{
	return (!(ft_isalnum(c) || c == '_'));
}

static void	handle_expansion_var(\
				char *new_str, size_t *new_str_len, int *i, t_info *info)
{
	char	*env_value;

	env_value = process_replace_expansion_var(info);
	ft_strlcat(new_str, env_value, MAX_ARGS);
	*new_str_len += ft_strlen(env_value);
	(*i)++;
}

static void	handle_normal_var(\
			char *new_str, char	*var, size_t *new_str_len, t_info *info)
{
	char	*env_value;

	env_value = process_replace_env_vars(var, info);
	ft_strlcat(new_str, env_value, MAX_ARGS);
	*new_str_len += ft_strlen(env_value);
}

static void	extract_var_name_from_input(\
		char *var, size_t *var_len, const char *input, int *i)
{
	*var_len = 0;
	while (*var_len < MAX_ARGS - 1 && input[*i] && !is_special_char(input[*i]))
	{
		var[(*var_len)++] = input[*i];
		(*i)++;
	}
	var[*var_len] = '\0';
}

char	*handler_dollar_sign_wihout_quotes(\
		char *new_str, char *input, t_info *info)
{
	size_t	new_str_len;
	size_t	var_len;
	char	var[MAX_ARGS];
	int		i;

	i = 0;
	new_str[0] = '\0';
	var_len = 0;
	new_str_len = 0;
	while (input[i])
	{
		if (input[i] == '$')
		{
			if (input[++i] == '?')
				handle_expansion_var(new_str, &new_str_len, &i, info);
			else
			{
				extract_var_name_from_input(var, &var_len, input, &i);
				handle_normal_var(new_str, var, &new_str_len, info);
			}
		}
		else
			new_str[new_str_len++] = input[i++];
	}
	return (new_str[new_str_len] = '\0', new_str);
}
