/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handler_operators_and_spaces.c                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: junsan <junsan@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/13 19:46:03 by junsan            #+#    #+#             */
/*   Updated: 2024/08/04 09:28:34 by junsan           ###   ########.fr       */
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
		if (cnt > 1 && *((*input) + 1) == **input)
		{
			add_token(list, *input, 2);
			(*input)++;
		}
		else
			add_token(list, *input, 1);
	}
	else if (cnt == 3 && ft_strncmp(*input, "<<<", 3) == 0)
	{
		add_token(list, *input, 3);
		*input += 2;
	}
	else
	{
		add_token(list, *input, cnt);
		*input += cnt - 1;
	}
	*start = *input + 1;
}

static void	handle_quotes_for_handle_cmd(\
			const char **input, bool *in_quotes, char *quote_char)
{
	if (**input == '"' || **input == '\'')
	{
		if (*in_quotes && **input == *quote_char)
			*in_quotes = false;
		else if (!*in_quotes)
		{
			*in_quotes = true;
			*quote_char = **input;
		}
	}
}

static void	handle_cmd(const char **input, const char **start, t_token **list)
{
	const char	*delims;
	bool		in_quotes;
	char		quote_char;

	in_quotes = false;
	quote_char = '\0';
	delims = DELIMS;
	while (**input && (in_quotes || \
		(!ft_isspace(**input) && !ft_strchr(delims, **input) && \
		**input != '(' && **input != ')')))
	{
		handle_quotes_for_handle_cmd(input, &in_quotes, &quote_char);
		(*input)++;
	}
	if (*input > *start)
		add_token(list, *start, *input - *start);
	if (**input && ft_strchr(delims, **input))
		handle_repeated_operators(input, start, list);
	else if (**input != '(' || **input != ')')
		*start = *input + 1;
	else
		*start = *input;
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
