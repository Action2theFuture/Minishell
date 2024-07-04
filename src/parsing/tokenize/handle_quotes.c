/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_quotes.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: junsan <junsan@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/13 19:44:46 by junsan            #+#    #+#             */
/*   Updated: 2024/07/04 15:25:05 by junsan           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	check_last_quote(const char *str)
{
	bool	in_single_quote;
	bool	in_double_quote;
	int		i;
	int		last_quote_index;

	in_single_quote = false;
	in_double_quote = false;
	last_quote_index = -1;
	i = 0;
	while (str[++i])
	{
		if (str[i] == '\'' && !in_double_quote)
		{
			in_single_quote = !in_single_quote;
			last_quote_index = i;
		}
		else if (str[i] == '"' && !in_single_quote)
		{
			in_double_quote = !in_double_quote;
			last_quote_index = i;
		}
		i++;
	}
	return (last_quote_index + 1);
}

static int	check_more_str(const char *str)
{
	int	i;

	i = 0;
	while (str[i])
	{
		if (!ft_isspace(*str) && \
			!ft_strchr(SHELL_METACHARS, *str))
			i++;
		else
			break ;
	}
	return (i);
}

void	handle_quotes(\
	const char **input, const char **start, t_token **list)
{
	int	cmd;

	*start = *input;
	cmd = check_last_quote(*input);
	cmd += check_more_str(*input + cmd);
	add_token(list, *input, cmd);
	*input += cmd;
	*start = *input + 1;
}
