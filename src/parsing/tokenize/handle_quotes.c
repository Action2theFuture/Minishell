/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_quotes.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: junsan <junsan@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/13 19:44:46 by junsan            #+#    #+#             */
/*   Updated: 2024/07/17 11:14:32 by junsan           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static bool	is_in_quote(char c, bool *in_single_quote, bool *in_double_quote)
{
	if (c == '\'' && !(*in_double_quote))
	{
		*in_single_quote = !(*in_single_quote);
		return (true);
	}
	else if (c == '"' && !(*in_single_quote))
	{
		*in_double_quote = !(*in_double_quote);
		return (true);
	}
	return (false);
}

static int	check_last_quote(const char *str)
{
	bool	in_single_quote;
	bool	in_double_quote;
	int		i;
	int		last_quote_index;

	in_single_quote = false;
	in_double_quote = false;
	last_quote_index = -1;
	i = -1;
	while (str[i])
	{
		if (is_in_quote(str[i], &in_single_quote, &in_double_quote))
			last_quote_index = i;
		if ((!in_single_quote && !in_double_quote) && \
			ft_strchr(SHELL_METACHARS, str[i]))
			break ;
		i++;
	}
	if (in_double_quote || in_single_quote)
		return (last_quote_index);
	if (last_quote_index == -1)
		return (0);
	if (str[last_quote_index] != '\0')
		return (last_quote_index + 1);
	return (last_quote_index);
}

void	handle_quotes(\
	const char **input, const char **start, t_token **list)
{
	int	cmd;

	*start = *input;
	cmd = check_last_quote(*input);
	add_token(list, *input, cmd);
	*input += cmd;
	*start = *input + 1;
}
