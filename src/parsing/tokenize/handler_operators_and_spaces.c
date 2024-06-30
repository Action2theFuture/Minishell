/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handler_operators_and_spaces.c                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: junsan <junsan@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/13 19:46:03 by junsan            #+#    #+#             */
/*   Updated: 2024/06/29 13:02:54 by junsan           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	handle_repeated_operators(const char **input, t_token **list)
{
	int	cnt;

	cnt = count_repeated_chars(*input, **input);
	if (cnt < 3)
	{
		if (*((*input) + 1) == **input)
		{
			add_token(list, *input, 2);
			(*input)++;
		}
		else
			add_token(list, *input, 1);
	}
	else
	{
		add_token(list, *input, cnt);
		*input += cnt - 1;
	}
}

void	handle_operators_and_spaces(\
	const char **input, const char **start, t_token **list)
{
	const char	*delims;

	delims = DELIMS;
	if (ft_isspace(**input) || ft_strchr(delims, **input))
	{
		if (*input > *start)
			add_token(list, *start, *input - *start);
		if (ft_strchr(delims, **input))
			handle_repeated_operators(input, list);
		*start = *input + 1;
	}
}
