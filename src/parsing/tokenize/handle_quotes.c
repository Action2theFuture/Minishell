/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_quotes.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: junsan <junsan@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/13 19:44:46 by junsan            #+#    #+#             */
/*   Updated: 2024/08/01 14:34:13 by junsan           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	update_quote_state(\
			char c, bool *in_single_quote, bool *in_double_quote)
{
	if (c == '\'' && !(*in_double_quote))
		*in_single_quote = !(*in_single_quote);
	else if (c == '"' && !(*in_single_quote))
		*in_double_quote = !(*in_double_quote);
}

static bool	are_quotes_balanced(const char *str, int *i)
{
	bool	in_single_quote;
	bool	in_double_quote;
	int		first_char;
	int		last_char;

	in_single_quote = false;
	in_double_quote = false;
	first_char = str[0];
	while (str[++(*i)])
	{
		update_quote_state(str[*i], &in_single_quote, &in_double_quote);
		if (!in_single_quote && !in_double_quote && \
			ft_strchr(SHELL_OPERATORS, str[*i]))
		{
			if (first_char == str[*i - 1])
				return (*i -= 1, true);
			break ;
		}
	}
	last_char = str[ft_strlen(str) - 1];
	if (first_char == last_char)
		return (*i = ft_strlen(str) - 1, true);
	return (!in_single_quote && !in_double_quote);
}

int	handle_quotes(\
	const char **input, const char **start, t_token **list)
{
	int	i;

	i = -1;
	*start = *input;
	if (!are_quotes_balanced(*input, &i))
		return (UNCLOSED_QUOTE);
	if (i == -1)
		*start = *input + 1;
	else
	{
		if ((*input)[i] != '\0')
			i++;
		add_token(list, *input, i);
		*input += i;
		*start = *input;
	}
	return (-1);
}
