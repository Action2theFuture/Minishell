/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   quotes_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: junsan <junsan@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/27 08:33:49 by junsan            #+#    #+#             */
/*   Updated: 2024/08/06 16:51:04 by junsan           ###   ########.fr       */
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

bool	contains_unquoted_dollar(const char *input)
{
	bool	in_single_quotes;
	bool	in_double_quotes;
	int		i;

	in_single_quotes = false;
	in_double_quotes = false;
	i = -1;
	while (input[++i] != '\0')
	{
		if (input[i] == '\'' && !in_double_quotes)
			in_single_quotes = !in_single_quotes;
		else if (input[i] == '"' && !in_single_quotes)
			in_double_quotes = !in_double_quotes;
		else if (input[i] == '$' && !in_single_quotes && !in_double_quotes)
			return (true);
	}
	return (false);
}
