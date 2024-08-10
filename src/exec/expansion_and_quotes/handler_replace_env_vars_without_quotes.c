/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handler_replace_env_vars_without_quotes.c          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: junsan <junsan@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/11 13:30:34 by junsan            #+#    #+#             */
/*   Updated: 2024/08/05 20:02:19 by junsan           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	hadle_dollar_sign(t_handler_info *h_info, const char *input)
{
	if (input[++h_info->i] == '?')
		handle_expansion_var_without_quotes(h_info, '?');
	else if (input[h_info->i] == '_' && input[h_info->i + 1] == '\0')
		handle_expansion_var_without_quotes(h_info, '_');
	else
	{
		extract_var_name_from_input(h_info, input);
		handle_normal_var_without_quotes(h_info);
	}
}

char	*handler_dollar_sign_wihout_quotes(\
		char *new_str, char *input, t_info *info)
{
	t_handler_info	h_info;

	init_handler_info(&h_info, new_str, info);
	h_info.new_str[0] = '\0';
	while (input[h_info.i])
	{
		if (!h_info.in_single_quotes && \
			input[h_info.i] == '$' && input[h_info.i + 1] && \
			((ft_isalnum(input[h_info.i + 1]) || \
			input[h_info.i + 1] == '_' || \
			input[h_info.i + 1] == '?')))
			hadle_dollar_sign(&h_info, input);
		else
		{
			if (handle_quotes_from_input(&h_info, input))
				continue ;
			h_info.new_str[h_info.new_str_len++] = input[h_info.i++];
		}
	}
	return (h_info.new_str[h_info.new_str_len] = '\0', h_info.new_str);
}
