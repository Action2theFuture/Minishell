/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_subshell.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: junsan <junsan@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/13 19:45:20 by junsan            #+#    #+#             */
/*   Updated: 2024/07/29 09:19:04 by junsan           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	add_depth_token(const char **input, int *depth, t_token **tokens)
{
	if (**input == '(')
	{
		if (*(*input - 1) == '$')
			return (SYNTAX_ERROR);
		add_token(tokens, "(", 1);
		(*depth)++;
	}
	else if (*depth > 0 && **input == ')')
	{
		add_token(tokens, ")", 1);
		(*depth)--;
	}
	return (-1);
}

int	handle_open_subshell(\
	const char **input, int *depth, const char **start, t_token **list)
{
	if (*(*input + 1) == '(')
		return (SYNTAX_ERROR);
	*start = *input + 1;
	add_token(list, "(", 1);
	(*depth)++;
	return (-1);
}

int	handle_close_subshell(\
	const char **input, int *depth, const char **start, t_token **list)
{
	(*depth)--;
	if (*depth >= 0)
	{
		if (*(*input + 1) == ')')
			return (SYNTAX_ERROR);
		while (ft_isspace(**start))
			(*start)++;
		*start = *input + 1;
		add_token(list, ")", 1);
	}
	return (-1);
}
