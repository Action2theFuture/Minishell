/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing_quotes_in_cmd.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: junsan <junsan@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/29 13:19:33 by junsan            #+#    #+#             */
/*   Updated: 2024/06/29 16:56:30 by junsan           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static char	*remove_all_quotes(const char *input)
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

char	**parse_cmd_line_with_quotes(const char *input, int *cnt)
{
	const char	*ptr;
	char		**args;
	char		*arg;
	bool		in_quotes;
	char		quote_char;
	int			len;
	int			arg_idx;

	len = ft_strlen(input);
	args = (char **)malloc(sizeof(char *) * MAX_ARGS);
	arg = (char *)malloc(sizeof(char) * (len + 1));
	in_quotes = false;
	quote_char = '\0';
	arg_idx = 0;
	ptr = input;
	while (*ptr)
	{
		while (*ptr == ' ')
			ptr++;
		int arg_len = 0;
		if (*ptr == '"' || *ptr == '\'')
		{
			in_quotes = true;
			quote_char = *ptr++;
		}
		else
			in_quotes = false;
		while (*ptr && (in_quotes || *ptr != ' '))
		{
			if (in_quotes && *ptr == quote_char)
			{
				in_quotes = false;
				ptr++;
				break ;
			}
			arg[arg_len++] = *ptr++;
		}
		arg[arg_len] = '\0';
		if (arg_len > 0)
			args[arg_idx++] = remove_all_quotes(arg);
		if (*ptr == ' ' && !in_quotes)
			ptr++;
	}
	free(arg);
	*cnt = arg_idx;
	return (args);
}
