/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   list_to_array.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: junsan <junsan@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/19 12:42:24 by junsan            #+#    #+#             */
/*   Updated: 2024/07/18 15:54:20 by junsan           ###   ########.fr       */
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

static char	*conver_env_item_to_str(t_env *cur)
{
	size_t	name_len;
	size_t	content_len;
	size_t	total_len;
	char	*str;

	name_len = ft_strlen(cur->name);
	content_len = ft_strlen(cur->content);
	total_len = name_len + content_len + 2;
	str = (char *)malloc(sizeof(char) * total_len);
	if (str == NULL)
		return (perror("malloc error"), NULL);
	ft_strlcpy(str, cur->name, name_len + 1);
	ft_strlcat(str, "=", total_len);
	ft_strlcat(str, cur->content, total_len);
	return (str);
}

static char	**convert_list_to_array(t_env *cur, char **arr)
{
	int		idx;

	idx = 0;
	if (!cur)
		return (NULL);
	while (cur)
	{
		if (cur->content == NULL)
		{
			cur = cur->next;
			continue ;
		}
		arr[idx] = conver_env_item_to_str(cur);
		if (arr[idx] == NULL)
			return (clear_arr_in_middle(idx, arr), NULL);
		cur = cur->next;
		idx++;
	}
	return (arr[idx] = NULL, arr);
}

char	**list_to_array(t_env *env)
{
	char	**arr;
	size_t	size;

	if (env == NULL)
		return (NULL);
	size = env_size(env);
	arr = (char **)malloc(sizeof(char *) * (size + 1));
	if (arr == NULL)
	{
		perror("malloc");
		return (NULL);
	}
	return (convert_list_to_array(env, arr));
}

void	clear_env_arr(char **arr)
{
	int	i;

	i = -1;
	while (arr[++i])
		free(arr[i]);
	free(arr);
	arr = NULL;
}
