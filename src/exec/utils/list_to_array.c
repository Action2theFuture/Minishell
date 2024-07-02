/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   list_to_array.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: junsan <junsan@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/19 12:42:24 by junsan            #+#    #+#             */
/*   Updated: 2024/07/01 21:05:56 by junsan           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	clear_arr_in_middle(int idx, char **arr)
{
	while (idx >= 0)
	{
		free(arr[idx]);
		idx--;
	}
	free(arr);
}

static char	**convert_list_to_array(t_env *cur, char **arr)
{
	size_t	name_len;
	size_t	content_len;
	size_t	total_len;
	int		idx;

	idx = 0;
	while (cur)
	{
		name_len = ft_strlen(cur->name);
		content_len = ft_strlen(cur->content);
		total_len = name_len + content_len + 2;
		arr[idx] = (char *)malloc(sizeof(char) * total_len);
		if (arr[idx] == NULL)
			return (perror("malloc"), \
					clear_arr_in_middle(idx, arr), NULL);
		ft_strlcpy(arr[idx], cur->name, total_len);
		ft_strlcat(arr[idx], "=", total_len);
		ft_strlcat(arr[idx], cur->content, total_len);
		cur = cur->next;
		idx++;
	}
	arr[idx] = NULL;
	return (arr);
}

char	**list_to_array(t_env *env)
{
	char	**arr;

	if (env == NULL)
		return (NULL);
	arr = (char **)malloc(sizeof(char *) * (env_size(env) + 1));
	if (arr == NULL)
	{
		perror("malloc");
		return (NULL);
	}
	return (convert_list_to_array(env, arr));
}


void	clear_arr(char **arr)
{
	int	i;

	i = -1;
	while (arr[++i])
		free(arr[i]);
	free(arr);
}

