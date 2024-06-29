/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   valid_token_utils.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: junsan <junsan@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/28 16:17:02 by junsan            #+#    #+#             */
/*   Updated: 2024/06/28 17:23:14 by junsan           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

bool	check_logical_err(t_token *head)
{
	while (head)
	{
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
