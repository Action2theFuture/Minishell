/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_quotes.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: junsan <junsan@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/13 19:44:46 by junsan            #+#    #+#             */
/*   Updated: 2024/08/06 16:18:39 by junsan           ###   ########.fr       */
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

// SHELL_OPERATORS "()&|<>"
static bool	are_quotes_balanced(const char *str, int *i)
{
	bool	in_single_quote;
	bool	in_double_quote;
	int		first_char;
	int		last_char;

	in_single_quote = false;
	in_double_quote = false;
	first_char = str[0];
	while (str[*i])
	{
		update_quote_state(str[*i], &in_single_quote, &in_double_quote);
		if (!in_single_quote && !in_double_quote)
		{
			if (first_char == str[*i] && *i > 0 && \
				str[*i + 1] != '\'' && str[*i + 1] != '\"')
				return (*i += 1, true);
		}
		(*i)++;
	}
	last_char = str[ft_strlen(str) - 1];
	if (first_char == last_char)
		return (*i = ft_strlen(str) - 1, true);
	return (!in_single_quote && !in_double_quote);
}

// extra_quotes_idx is Index for additional post-parsing quote handling
// SHELL_METACHARS "\"\'()&|<>"
static int	calculate_token_len(const char **input, int *i)
{
	int	extra_quotes_idx;
	int	prev_i;

	while ((*input)[*i] && !ft_isspace((*input)[*i]))
	{
		prev_i = *i;
		while ((*input)[*i] && !ft_isspace((*input)[*i]) && \
		!ft_strchr(SHELL_METACHARS, (*input)[*i]))
			(*i)++;
		if (prev_i == *i)
			return (SYNTAX_ERROR);
		extra_quotes_idx = 0;
		if (are_quotes_balanced(&(*input)[*i], &extra_quotes_idx))
			*i += extra_quotes_idx;
	}
	return (SUCCESS);
}

// printf("input : %s\n", *input);
int	handle_quotes(\
	const char **input, const char **start, t_token **list)
{
	int			i;
	int			status;

	i = 0;
	*start = *input;
	if (!are_quotes_balanced(*input, &i))
		return (UNCLOSED_QUOTE);
	if (i == 0)
		*start = *input + 1;
	else
	{
		status = calculate_token_len(input, &i);
		if (status != SUCCESS)
			return (status);
		add_token(list, *input, i);
		*input += i;
		*start = *input;
	}
	return (-1);
}
