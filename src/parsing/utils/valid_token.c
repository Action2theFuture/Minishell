/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   valid_token.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: junsan <junsan@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/23 20:07:50 by junsan            #+#    #+#             */
/*   Updated: 2024/06/26 21:43:05 by junsan           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static bool	check_quotes_in_tokens(t_token *head)
{
	char	quote;
	char	*data;
	int		i;

	quote = 0;
	while (head)
	{
		i = -1;
		data = head->data;
		while (data[++i])
		{
			if (data[i] == '"' || data[i] == '\'')
			{
				if (quote == 0)
					quote = data[i];
				else if (quote == data[i])
					quote = 0;
			}
		}
		head = head->next;
	}
	return (quote == 0);
}

static bool	check_subshell_closed(t_token *head)
{
	int		depth;

	depth = 0;
	while (head)
	{
		if (head->data[0] == '(' && ft_strlen(head->data) == 1)
			depth++;
		if (head->data[0] == ')' && ft_strlen(head->data) == 1)
			depth--;
		head = head->next;
	}
	return (depth == 0);
}

int	valid_token(t_token *head)
{
	if (!check_quotes_in_tokens(head))
		return (UNCLOSED_QUOTE);
	if (!check_subshell_closed(head))
		return (TOKEN_ERROR);
	return (-1);
}
