/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing_quotes_in_cmd.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: junsan <junsan@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/29 13:19:33 by junsan            #+#    #+#             */
/*   Updated: 2024/06/30 21:59:07 by junsan           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

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

static void	handle_quotes_in_args(\
			const char **ptr, bool *in_single_quotes, bool *in_double_quotes)
{
	if (*in_double_quotes)
		*in_double_quotes = false;
	else if (!*in_single_quotes)
		*in_double_quotes = true;
	(*ptr)++;
}

static void	parse_arg(\
			const char **ptr, char *arg, int *arg_len, bool *in_quotes)
{
	*arg_len = 0;
	while (**ptr && (in_quotes[0] || in_quotes[1] || **ptr != ' '))
	{
		if (**ptr == '"')
			handle_quotes_in_args(ptr, &in_quotes[0], &in_quotes[1]);
		else if (**ptr == '\'')
		{
			if (in_quotes[0])
				in_quotes[0] = false;
			else if (!in_quotes[1])
				in_quotes[0] = true;
			else
				arg[(*arg_len)++] = **ptr;
			(*ptr)++;
		}
		else
			arg[(*arg_len)++] = *(*ptr)++;
	}
	arg[*arg_len] = '\0';
}

// arg_idx[0] -> arg_len arg_idx[1] -> arg_idx;
// in_quotes[0] -> in_single_quotes in_quotes[1] -> in_double_quotes
char	**parse_cmd_line_with_quotes(const char *input, int *cnt)
{
	const char	*ptr;
	char		**args;
	char		*arg;
	bool		in_quotes[2];
	int			arg_idx[2];

	args = (char **)malloc(sizeof(char *) * MAX_ARGS);
	arg = (char *)malloc(sizeof(char) * (ft_strlen(input) + 1));
	if (!args || !arg)
		return (perror("malloc error"), NULL);
	in_quotes[0] = false;
	in_quotes[1] = false;
	arg_idx[0] = 0;
	arg_idx[1] = 0;
	ptr = input;
	while (*ptr)
	{
		while (*ptr == ' ')
			ptr++;
		parse_arg(&ptr, arg, &arg_idx[0], in_quotes);
		if (arg_idx[0] > 0)
			args[arg_idx[1]++] = ft_strdup(arg);
		ft_memset(arg, '\0', ft_strlen(arg));
	}
	return (free(arg), args[arg_idx[1]++] = NULL, *cnt = arg_idx[1] - 1, args);
}
