/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   subshell_and_quote_str.c                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: junsan <junsan@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/02 11:44:55 by junsan            #+#    #+#             */
/*   Updated: 2024/07/02 11:48:56 by junsan           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	update_quote_state(\
					char ch, bool *in_single_quotes, bool *in_double_quotes)
{
	if (ch == '\'' && !(*in_double_quotes))
		*in_single_quotes = !(*in_single_quotes);
	else if (ch == '"' && !(*in_single_quotes))
		*in_double_quotes = !(*in_double_quotes);
}

static bool	update_balance(char ch, int *balance)
{
	if (ch == '(')
		(*balance)++;
	else if (ch == ')')
	{
		(*balance)--;
		if (*balance < 0)
			return (false);
	}
	return (true);
}

bool	are_parentheses_balanced(const char *input)
{
	int		balance;
	int		i;
	bool	in_single_quotes;
	bool	in_double_quotes;

	in_single_quotes = false;
	in_double_quotes = false;
	i = -1;
	balance = 0;
	while (input[++i])
	{
		update_quote_state(input[i], &in_single_quotes, &in_double_quotes);
		if (!in_single_quotes && !in_double_quotes)
		{
			if (!update_balance(input[i], &balance))
				return (false);
		}
	}
	return (balance == 0);
}
