/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   subshell_utils_2.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: junsan <junsan@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/13 11:52:53 by junsan            #+#    #+#             */
/*   Updated: 2024/08/13 11:57:47 by junsan           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static bool	check_inner_parentheses(\
				const char *str, int start, int end)
{
	int	open_count;

	open_count = 0;
	while (start <= end)
	{
		if (str[start] == '(')
			open_count++;
		else if (str[start] == ')')
		{
			open_count--;
			if (open_count < 0)
				return (false);
		}
		start++;
	}
	return (true);
}

// exaple : (           ( (spaces between parenthes)
bool	is_surrounded_by_parentheses(const char *str, int n)
{
	int	len;
	int	i;
	int	j;

	len = (int)ft_strlen(str);
	i = 0;
	while (i < len && str[i] == '(')
		i++;
	if (i < n)
		return (false);
	j = len - 1;
	while (j >= 0 && str[j] == ')')
		j--;
	if (len - j - 1 < n)
		return (false);
	return (check_inner_parentheses(str, i, j));
}
