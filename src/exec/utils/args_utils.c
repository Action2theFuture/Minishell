/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   args_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: junsan <junsan@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/19 14:48:46 by junsan            #+#    #+#             */
/*   Updated: 2024/08/01 10:18:27 by junsan           ###   ########.fr       */
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

	if (args == NULL)
		return ;
	i = -1;
	while (args[++i])
		free(args[i]);
	free(args);
	args = NULL;
}

char	**allocate_null_and_cmd_chunk(char **cmd, int cmd_cnt)
{
	char	**chunk;
	int		i;

	chunk = (char **)malloc(sizeof(char *) * (cmd_cnt + 1));
	if (chunk == NULL)
		return (perror("malloc"), NULL);
	i = -1;
	while (cmd[++i])
		chunk[i] = ft_strdup(cmd[i]);
	chunk[cmd_cnt] = NULL;
	if (cmd)
		free_args(cmd);
	return (chunk);
}

char	**copy_str_arr(char **src)
{
	size_t	cnt;
	char	**dest;
	int		i;
	int		j;

	if (src == NULL)
		return (NULL);
	cnt = count_strings(src);
	dest = (char **)malloc(sizeof(char *) * (cnt + 1));
	if (dest == NULL)
		return (perror("malloc error"), NULL);
	i = -1;
	while (++i < (int)cnt)
	{
		dest[i] = ft_strdup(src[i]);
		if (dest[i] == NULL)
		{
			perror("strdup error");
			j = -1;
			while (++j < i)
				free(dest[j]);
			return (free(dest), NULL);
		}
	}
	return (dest[cnt] = NULL, dest);
}

int	count_strings(char **arr)
{
	int	cnt;

	cnt = 0;
	while (arr[cnt] != NULL)
		cnt++;
	return (cnt);
}
