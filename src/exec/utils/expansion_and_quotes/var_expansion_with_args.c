/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   var_expansion_with_args.c                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: junsan <junsan@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/23 19:03:26 by junsan            #+#    #+#             */
/*   Updated: 2024/07/12 15:28:17 by junsan           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	init_quote_info(t_quote_info *quote_info)
{
	quote_info->in_single_quotes = false;
	quote_info->in_double_quotes = false;
	quote_info->tmp_str_len = 0;
	quote_info->new_str_len = 0;
	ft_memset(quote_info->tmp_str, 0, MAX_ARGS);
}

static char	*expand_and_strip_quotes_in_str(const char *str, t_info *info)
{
	t_quote_info	quote_info;
	char			*res;

	res = (char *)malloc(sizeof(char) * MAX_ARGS);
	if (!res)
		return (perror("malloc error"), NULL);
	if (find_unquoted_dollar(str))
		return (handler_dollar_sign_wihout_quotes(res, (char *)str, info), \
		free((char *)str), res);
	init_quote_info(&quote_info);
	info->quote_info = &quote_info;
	res = process_expand_strip_quotes(res, str, info);
	free((char *)str);
	return (res);
}

void	expand_and_strip_quotes_in_args(char **args, t_info *info)
{
	char	*new_arg;
	int		i;

	i = -1;
	while (args[++i])
	{
		new_arg = expand_and_strip_quotes_in_str(args[i], info);
		args[i] = new_arg;
	}
}
