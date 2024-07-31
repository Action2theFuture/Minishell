/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   quotes_str.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: junsan <junsan@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/27 08:39:13 by junsan            #+#    #+#             */
/*   Updated: 2024/07/23 20:27:20 by junsan           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	remove_single_quotes(char *str)
{
	size_t	len;

	len = ft_strlen(str);
	if (len >= 2 && ((str[0] == '\'' && str[len - 1] == '\'')))
	{
		ft_memmove(str, str + 1, len - 2);
		str[len - 2] = '\0';
	}
}

void	remove_double_quotes(char *str)
{
	size_t	len;

	len = ft_strlen(str);
	if (len >= 2 && ((str[0] == '"' && str[len - 1] == '"')))
	{
		ft_memmove(str, str + 1, len - 2);
		str[len - 2] = '\0';
	}
}

void	remove_quotes(char *str)
{
	size_t	len;

	len = ft_strlen(str);
	if (len >= 2 && ((str[0] == '"' && str[len - 1] == '"') \
					|| (str[0] == '\'' && str[len - 1] == '\'')))
	{
		ft_memmove(str, str + 1, len - 2);
		str[len - 2] = '\0';
	}
}

void	remove_empty_quotes(char *str)
{
	int	len;
	int	i;
	int	j;

	len = ft_strlen(str);
	i = 0;
	j = 0;
	while (i < len)
	{
		if ((str[i] == '"' && str[i + 1] == '"') || \
			(str[i] == '\'' && str[i + 1] == '\''))
		{
			i += 2;
			continue ;
		}
		str[j++] = str[i++];
	}
	str[j] = '\0';
}
