/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handler_operators_and_spaces.c                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: junsan <junsan@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/13 19:46:03 by junsan            #+#    #+#             */
/*   Updated: 2024/06/30 16:54:52 by junsan           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	handle_repeated_operators(\
				const char **input, const char **start, t_token **list)
{
	int	cnt;

	cnt = count_repeated_chars(*input, **input);
	if (cnt < 3)
	{
		if (*((*input) + 1) == **input)
		{
			add_token(list, *input, 2);
			(*input)++;
		}
		else
			add_token(list, *input, 1);
	}
	else
	{
		add_token(list, *input, cnt);
		*input += cnt - 1;
	}
	*start = *input + 1;
}

static void	handle_cmd(const char **input, const char **start, t_token **list)
{
	bool	in_quotes;
	char	quote_char;

	in_quotes = false;
	quote_char = '\0';
	while (**input && (in_quotes || !ft_isspace(**input)))
	{
		if (**input == '"' || **input == '\'')
		{
			if (in_quotes && **input == quote_char)
				in_quotes = false;
			else if (!in_quotes)
			{
				in_quotes = true;
				quote_char = **input;
			}
		}
		(*input)++;
	}
	if (*input > *start)
		add_token(list, *start, *input - *start);
	*start = *input + 1;
}

void	handle_operators_and_spaces(\
	const char **input, const char **start, t_token **list)
{
	const char	*delims;

	delims = DELIMS;
	while (**input && ft_isspace(**input))
		(*input)++;
	*start = *input;
	if (ft_strchr(delims, **input))
		handle_repeated_operators(input, start, list);
	else
		handle_cmd(input, start, list);
}
