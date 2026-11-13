/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strlcat.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: junsan <junsan@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/10 22:41:19 by junsan            #+#    #+#             */
/*   Updated: 2023/11/12 19:11:16 by junsan           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

size_t	ft_strlcat(char *dst, const char *src, size_t len)
{
	size_t	i;
	size_t	j;

	i = 0;
	j = 0;
	while (i < len && dst[i])
		i++;
	while ((i + j + 1) < len && src[j])
	{
		dst[i + j] = src[j];
		j++;
	}
	if (i != len)
	{
		dst[i + j] = '\0';
	}
	return (i + ft_strlen(src));
}
/*
#include <stddef.h>
#include "libft.h"

size_t	ft_strlcat(char *dst, const char *src, size_t len)
{
	size_t	i;
	size_t	ret;

	if (!len)
		return (0);
	if (!dst && !src)
		return (0);
	i = 0;
	while (*dst && i < len)
	{
		++dst;
		++i;
	}
	ret = ft_strlcpy(dst, src, len - i);
	return (ret + i);
}*/
