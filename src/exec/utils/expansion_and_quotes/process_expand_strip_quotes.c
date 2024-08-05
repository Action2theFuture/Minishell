/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   process_expand_strip_quotes.c                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: junsan <junsan@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/11 12:57:33 by junsan            #+#    #+#             */
/*   Updated: 2024/08/05 18:14:02 by junsan           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	process_single_quote(\
		const char *input, int *i, char *new_str, t_info *info)
{
	t_quote_info	*quote_info;
	char			res[MAX_ARGS];

	quote_info = info->quote_info;
	if (quote_info->in_single_quotes)
	{
		quote_info->tmp_str[quote_info->tmp_str_len++] = input[*i];
		quote_info->tmp_str[quote_info->tmp_str_len] = '\0';
		replace_env_vars(quote_info->tmp_str, res, info);
		ft_strlcat(new_str, res, MAX_ARGS);
		quote_info->new_str_len += ft_strlen(res);
		quote_info->tmp_str_len = 0;
	}
	else
		quote_info->tmp_str[quote_info->tmp_str_len++] = input[*i];
	quote_info->in_single_quotes = !quote_info->in_single_quotes;
}

static void	process_double_quote(\
		const char *input, int *i, char *new_str, t_info *info)
{
	t_quote_info	*quote_info;
	char			res[MAX_ARGS];

	quote_info = info->quote_info;
	if (quote_info->in_double_quotes)
	{
		quote_info->tmp_str[quote_info->tmp_str_len++] = input[*i];
		quote_info->tmp_str[quote_info->tmp_str_len] = '\0';
		replace_env_vars(quote_info->tmp_str, res, info);
		ft_strlcat(new_str, res, MAX_ARGS);
		quote_info->new_str_len += ft_strlen(res);
		quote_info->tmp_str_len = 0;
	}
	else
		quote_info->tmp_str[quote_info->tmp_str_len++] = input[*i];
	quote_info->in_double_quotes = !quote_info->in_double_quotes;
}

static void	process_regular_char(\
		const char input_char, char *new_str, t_quote_info *quote_info)
{
	char	tmp[2];

	tmp[0] = input_char;
	tmp[1] = '\0';
	ft_strlcat(new_str, tmp, MAX_ARGS);
	quote_info->new_str_len++;
}

char	*process_expand_strip_quotes(\
				char *new_str, const char *input, t_info *info)
{
	t_quote_info	*quote_info;
	int				i;

	new_str[0] = '\0';
	quote_info = info->quote_info;
	i = -1;
	while (input[++i])
	{
		if (input[i] == '\'' && !quote_info->in_double_quotes)
			process_single_quote(input, &i, new_str, info);
		else if (input[i] == '"' && !quote_info->in_single_quotes)
			process_double_quote(input, &i, new_str, info);
		else if (quote_info->in_single_quotes || quote_info->in_double_quotes)
		{
			quote_info->tmp_str[quote_info->tmp_str_len++] = input[i];
			quote_info->tmp_str[quote_info->tmp_str_len] = '\0';
		}
		else
			process_regular_char(input[i], new_str, quote_info);
	}
	return (new_str);
}
