/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   valid_token_err_2.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: junsan <junsan@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/28 16:17:02 by junsan            #+#    #+#             */
/*   Updated: 2024/08/13 12:50:29 by junsan           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

bool	check_logical_err(t_token *head)
{
	while (head)
	{
		if (head->data[0] == '&' && ft_strlen(head->data) == 1)
			return (true);
		if (head->type == LOGICAL && ft_strlen(head->data) > 2)
			return (true);
		head = head->next;
	}
	return (false);
}

bool	is_cmd_valid(const char *cmd)
{
	if (!cmd || *cmd == '\0')
		return (false);
	while (*cmd)
	{
		if (!ft_isspace((unsigned char)*cmd))
			return (true);
		cmd++;
	}
	return (false);
}

bool	is_surrounded_by_double_parentheses(t_token *head)
{
	bool	have_open_paren;

	have_open_paren = false;
	if (!head)
		return (false);
	while (head)
	{
		if (head->type == SUBSHELL && head->data[0] == '(' && \
			head->next && head->next->type == SUBSHELL && \
			head->next->data[0] == '(')
			have_open_paren = true;
		if (head->type == SUBSHELL && head->data[0] == ')' && \
			head->next && head->next->type == SUBSHELL && \
			head->next->data[0] == ')')
		{
			if (have_open_paren)
				return (true);
		}
		head = head->next;
	}
	return (false);
}
