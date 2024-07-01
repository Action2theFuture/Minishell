/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_quotes.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: junsan <junsan@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/13 19:44:46 by junsan            #+#    #+#             */
/*   Updated: 2024/06/30 17:34:54 by junsan           ###   ########.fr       */
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
	return (cmd + 1);
}

static int	check_more_str(const char *str)
{
	int	i;

	i = 0;
	while (str[i])
	{
		if (!ft_isspace(*str) && \
			!ft_strchr(SHELL_METACHARS, *str))
			i++;
		else
			break ;
	}
	return (i);
}

void	handle_quotes(\
	const char **input, const char **start, t_token **list)
{
	int	cmd;

	*start = *input;
	cmd = check_last_quote(*input);
	cmd += check_more_str(*input + cmd);
	add_token(list, *input, cmd);
	*input += cmd;
	*start = *input + 1;
}
