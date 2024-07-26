/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   normalize_spaces.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: junsan <junsan@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/26 09:38:51 by junsan            #+#    #+#             */
/*   Updated: 2024/07/26 09:52:06 by junsan           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	skip_initial_spaces(const char *str)
{
	int		i;

	i = 0;
	while (ft_isspace((unsigned char)str[i]))
		i++;
	return (i);
}

static void	remove_trailing_space(char	*result, int *result_idx)
{
	if (*result_idx > 0 && \
		ft_isspace((unsigned char)result[*result_idx - 1]))
		(*result_idx)--;
	result[*result_idx] = '\0';
}

static void	remove_extra_spaces(char *result, const char *str)
{
	bool	in_space;
	int		i;
	int		j;

	i = skip_initial_spaces(str);
	j = 0;
	while (str[i] != '\0')
	{
		if (ft_isspace((unsigned char)str[i]))
		{
			if (!in_space)
			{
				result[j++] = ' ';
				in_space = true;
			}
		}
		else
		{
			result[j++] = str[i];
			in_space = false;
		}
		i++;
	}
	remove_trailing_space(result, &j);
}

char	*normalize_spaces(const char *str)
{
	char	*result;

	if (!str)
		return (NULL);
	result = (char *)malloc(sizeof(char) * (ft_strlen(str) + 1));
	if (!result)
		return (perror("malloc error"), NULL);
	remove_extra_spaces(result, str);
	return (result);
}
