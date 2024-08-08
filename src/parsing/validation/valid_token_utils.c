/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   valid_token_utils.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: junsan <junsan@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/28 17:25:07 by junsan            #+#    #+#             */
/*   Updated: 2024/08/06 00:29:10 by junsan           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

bool	check_quotes_in_tokens(t_token *head)
{
	char	quote;
	char	*data;
	int		i;

	while (head)
	{
		quote = 0;
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
		if (quote != 0)
			return (false);
		head = head->next;
	}
	return (true);
}

bool	check_subshell_closed(t_token *head)
{
	int		depth;

	depth = 0;
	if (head && head->data[0] == '(' && !head->next)
		return (false);
	while (head)
	{
		if ((head->data[0] == '(' && ft_strlen(head->data) == 1) && \
			(head->next->data[0] == ')' && ft_strlen(head->next->data) == 1))
			return (false);
		if (head->data[0] == '(' && ft_strlen(head->data) == 1)
			depth++;
		if (head->data[0] == ')' && ft_strlen(head->data) == 1)
			depth--;
		head = head->next;
	}
	return (depth == 0);
}
