/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_subshell.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: junsan <junsan@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/13 19:45:20 by junsan            #+#    #+#             */
/*   Updated: 2024/06/26 21:28:22 by junsan           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	handle_open_subshell(\
	const char **input, int *depth, const char **start, t_token **list)
{
	if (*depth == 0)
	{
		(*input)++;
		*start = *input;
		add_token(list, "(", 1);
	}
	(*depth)++;
}

void	handle_close_subshell(\
	const char **input, int *depth, const char **start, t_token **list)
{
	(*depth)--;
	if (*depth == 0)
	{
		while (ft_isspace(**start))
			(*start)++;
		if (*input - *start != 0)
			add_token(list, *start, *input - *start);
		add_token(list, ")", 1);
		*start = *input + 1;
	}
}
