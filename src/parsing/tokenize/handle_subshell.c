/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_subshell.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: junsan <junsan@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/13 19:45:20 by junsan            #+#    #+#             */
/*   Updated: 2024/07/01 17:25:17 by junsan           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	handle_open_subshell(\
	const char **input, int *depth, const char **start, t_token **list)
{
	*start = *input + 1;
	add_token(list, "(", 1);
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
		*start = *input + 1;
		add_token(list, ")", 1);
	}
}
