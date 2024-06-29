/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   list_to_array.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: junsan <junsan@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/19 12:42:24 by junsan            #+#    #+#             */
/*   Updated: 2024/06/26 13:31:19 by junsan           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	clear_arr(int len, char **arr)
{
	int	i;

	i = -1;
	while (++i < len)
		free(arr[i]);
	free(arr);
}

static char	**convert_list_to_array(t_env *cur, char **arr)
{
	size_t	name_len;
	size_t	content_len;
	int		idx;

	idx = -1;
	while (cur)
	{
		name_len = ft_strlen(cur->name);
		content_len = ft_strlen(cur->content);
		arr[++idx] = (char *)malloc(sizeof(char *) * \
				(name_len + content_len + 2));
		if (arr[idx] == NULL)
			return (perror("malloc"), \
					clear_arr(idx, arr), NULL);
		ft_strlcpy(arr[idx], cur->name, name_len + 1);
		ft_strlcat(arr[idx], "=", name_len + 2);
		ft_strlcat(arr[idx], cur->content, name_len + content_len + 2);
		cur = cur->next;
	}
	arr[idx] = NULL;
	return (arr);
}

char	**list_to_array(t_env *env)
{
	char	**arr;

	if (env == NULL)
		return (NULL);
	arr = (char **)malloc(sizeof(char *) * env_size(env));
	if (arr == NULL)
	{
		perror("malloc");
		return (NULL);
	}
	return (convert_list_to_array(env, arr));
}
