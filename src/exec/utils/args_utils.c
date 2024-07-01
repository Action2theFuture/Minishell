/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   args_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: junsan <junsan@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/19 14:48:46 by junsan            #+#    #+#             */
/*   Updated: 2024/07/01 20:11:21 by junsan           ###   ########.fr       */
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

char	**allocate_null_and_cmd_chunk(char **cmd, int cmd_cnt)
{
	char	**chunk;
	int		i;

	cmd_cnt = 0;
	while (cmd[cmd_cnt])
		cmd_cnt++;
	chunk = (char **)malloc(sizeof(char *) * cmd_cnt);
	if (chunk == NULL)
	{
		perror("malloc");
		return (NULL);
	}
	i = -1;
	while (++i < cmd_cnt)
		chunk[i] = ft_strdup(cmd[i]);
	chunk[cmd_cnt] = NULL;
	return (chunk);
}
