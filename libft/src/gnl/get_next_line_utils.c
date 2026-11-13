/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line_utils.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: junsan <junsan@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/16 13:09:28 by junsan            #+#    #+#             */
/*   Updated: 2024/05/12 18:36:32 by junsan           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line.h"
#include "libft.h"

char	*gnl_strchr(const char *s, size_t len)
{
	size_t	i;
	char	newline;
	char	*str;

	newline = '\n';
	if (!s)
		return (NULL);
	str = (char *)s;
	i = 0;
	while (i <= len)
	{
		if (str[i] == newline)
			return ((char *)&str[i]);
		i++;
	}
	return (NULL);
}

char	*gnl_strjoin(char *s1, char *s2, size_t len, size_t rd)
{
	size_t	i;
	size_t	j;
	char	*str;

	if (!s1)
		return (ft_strdup(s2));
	if (!s2)
		return (NULL);
	str = (char *)malloc(sizeof(char) * (len + 1));
	if (!str)
		return (NULL);
	i = -1;
	while (++i < len - rd)
		str[i] = s1[i];
	j = 0;
	while (j < rd)
		str[i++] = s2[j++];
	str[i] = '\0';
	free(s1);
	s1 = NULL;
	return (str);
}
