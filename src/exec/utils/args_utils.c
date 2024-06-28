/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   args_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: junsan <junsan@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/19 14:48:46 by junsan            #+#    #+#             */
/*   Updated: 2024/06/27 08:34:16 by junsan           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/*
static void	clear_flag(char **str, int idx)
{
	int	i;

	i = -1;
	while (++i < idx)
		free(str[i]);
	free(str);
}

static char **split_flags(const char *flags)
{
	size_t 	len;
	int		idx;
	int		i;
	char	**res;

	len = ft_strlen(flags);
	res = (char **)malloc(sizeof(char *) * (len * 2 + 1));
	if (res == NULL)
	{
		perror("malloc error");
		return (NULL);
	}
	idx = -1;
	i = = 1;
	while (++i < (int)len)
	{
		res[++idx] = (char *)malloc(sizeof(char) * 3);
		if (res[idx] == NULL)
		{
			perror("malloc error");
			clear_flag(str, idx);
			return (NULL);
		}
	}
	res[idx] = NULL;
	return (res);
}
*/
void	free_args(char **args)
{
	int	i;

	i = -1;
	if (args)
	{
		while (args[++i])
			free(args[i]);
		free(args);
	}
}

char	**allocate_null_and_cmd_chunk(const char *cmd)
{
	char	**chunk;

	chunk = (char **)malloc(sizeof(char *) * 2);
	if (chunk == NULL)
	{
		perror("malloc");
		return (NULL);
	}
	chunk[0] = ft_strdup(cmd);
	chunk[1] = NULL;
	return (chunk);
}
