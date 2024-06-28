/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_quotes.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: junsan <junsan@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/13 19:44:46 by junsan            #+#    #+#             */
/*   Updated: 2024/06/26 23:05:15 by junsan           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	check_last_quote(const char *str)
{
	int	i;
	int	cmd;

	i = -1;
	cmd = 0;
	while (str[++i])
	{
		if (str[i] == '"' || str[i] == '\'')
		{
			if (cmd < i)
				cmd = i;
		}
	}
	return (cmd);
}

void	handle_quotes(\
	const char **input, const char **start, t_token **list)
{
	int	cmd;

	if (**input == '"' || **input == '\'')
	{
		*start = *input;
		cmd = check_last_quote(*input);
		add_token(list, *input, cmd + 1);
		*input += cmd;
		*start = *input + 1;
	}
}
