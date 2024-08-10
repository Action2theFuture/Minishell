/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   quotes_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: junsan <junsan@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/27 08:33:49 by junsan            #+#    #+#             */
/*   Updated: 2024/08/06 18:53:55 by junsan           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	remove_consecutive_double_quotes(char *str)
{
	int	len;
	int	i;
	int	j;

	len = ft_strlen(str);
	if ((len == 2 && ft_strncmp(str, "\"\"", 2) == 0) || \
		(len == 1 && ft_strncmp(str, "\"", 1) == 0))
	{
		str[0] = '\0';
		return ;
	}
	i = -1;
	j = 0;
	while (++i < len)
	{
		if (!(str[i] == '\"' && str[i + 1] == '\"'))
			str[j++] = str[i];
	}
	str[j] = '\0';
}

void	remove_consecutive_double_quotes_from_args(char **args)
{
	int		i;

	if (args == NULL || args[0] == NULL)
		return ;
	i = -1;
	while (args[++i])
		remove_consecutive_double_quotes(args[i]);
}

void	remove_double_quotes_from_args(char **args)
{
	int		i;

	if (args == NULL || args[0] == NULL)
		return ;
	i = -1;
	while (args[++i])
	{
		while (args[i][0] == '\"' && args[i][ft_strlen(args[i] - 1)] == '\"')
			remove_surrounding_double_quotes(args[i]);
	}
}

void	remove_single_quotes_from_args(char **args)
{
	int		i;

	if (args == NULL || args[0] == NULL)
		return ;
	i = -1;
	while (args[++i])
	{
		while (args[i][0] == '\'' && args[i][ft_strlen(args[i] - 1)] == '\'')
			remove_surrounding_single_quotes(args[i]);
	}
}

char	*remove_all_quotes(const char *input)
{
	int		len;
	char	*res;
	int		i;
	int		j;

	len = ft_strlen(input);
	res = (char *)malloc(sizeof(char) * (len + 1));
	i = -1;
	j = 0;
	while (++i < len)
	{
		if (input[i] != '"' && input[i] != '\'')
			res[j++] = input[i];
	}
	res[j] = '\0';
	return (res);
}
