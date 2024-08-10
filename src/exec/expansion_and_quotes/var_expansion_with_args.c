/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   var_expansion_with_args.c                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: junsan <junsan@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/23 19:03:26 by junsan            #+#    #+#             */
/*   Updated: 2024/08/06 18:54:41 by junsan           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static bool	contains_unquoted_dollar(const char *input)
{
	bool	in_single_quotes;
	bool	in_double_quotes;
	int		i;

	in_single_quotes = false;
	in_double_quotes = false;
	i = -1;
	while (input[++i] != '\0')
	{
		if (input[i] == '\'' && !in_double_quotes)
			in_single_quotes = !in_single_quotes;
		else if (input[i] == '"' && !in_single_quotes)
			in_double_quotes = !in_double_quotes;
		else if (input[i] == '$' && !in_single_quotes && !in_double_quotes)
			return (true);
	}
	return (false);
}

static void	init_quote_info(t_quote_info *quote_info)
{
	quote_info->in_single_quotes = false;
	quote_info->in_double_quotes = false;
	quote_info->tmp_str_len = 0;
	quote_info->new_str_len = 0;
	ft_memset(quote_info->tmp_str, 0, MAX_ARGS);
}

static char	*expand_and_strip_quotes_in_str(char *str, t_info *info)
{
	t_quote_info	quote_info;
	char			*res;

	res = ft_calloc(sizeof(char), MAX_ARGS);
	if (!res)
		return (perror("malloc error"), NULL);
	if (contains_unquoted_dollar(str))
	{
		handler_dollar_sign_wihout_quotes(res, str, info);
		if (str)
			free(str);
		return (res);
	}
	init_quote_info(&quote_info);
	info->quote_info = &quote_info;
	res = process_expand_strip_quotes(res, str, info);
	free(str);
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
