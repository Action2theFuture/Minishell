/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_split.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: junsan <junsan@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/10 22:40:41 by junsan            #+#    #+#             */
/*   Updated: 2023/11/14 18:16:56 by junsan           ###   ########.fr       */
/*   Updated: 2023/11/14 12:27:19 by junsan           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static size_t	count_word(const char *str, char c)
{
	int	i;
	int	cnt_arr;

	i = -1;
	cnt_arr = 0;
	if (!str[0])
		return (0);
	while (str[++i])
	{
		if (!i && str[i] != c)
			cnt_arr++;
		if (str[i] == c && str[i + 1] && str[i + 1] != c)
			cnt_arr++;
	}
	return (cnt_arr);
}

static void	ft_free(char **arr)
{
	int	i;

	i = -1;
	while (arr[++i])
	{
		free(arr[i]);
		arr[i] = 0;
	}
	free(arr);
	arr = 0;
}

static char	*ft_strdup_result(char *src, size_t n)
{
	char	*str;
	size_t	i;

	str = (char *)malloc(sizeof(char) * (n + 1));
	if (!str)
		return (NULL);
	i = 0;
	while (i < n)
	{
		str[i] = src[i];
		i++;
	}
	str[i] = 0;
	return (str);
}

char	**ft_split(char const *str, char c)
{
	char	**str_arr;
	size_t	i;
	size_t	j;
	size_t	tmp;

	if (!str)
		return (NULL);
	str_arr = (char **)malloc(sizeof(char *) * (count_word(str, c) + 1));
	if (!str_arr)
		return (NULL);
	i = 0;
	j = 0;
	while (i < count_word(str, c) && str[j])
	{
		while (str[j] == c)
			j++;
		tmp = j;
		while ((str[j] != c) && str[j])
			j++;
		str_arr[i] = ft_strdup_result(&((char *)str)[tmp], j - tmp);
		if (!str_arr[i++])
			ft_free(str_arr);
	}
	str_arr[i] = NULL;
	return (str_arr);
}
