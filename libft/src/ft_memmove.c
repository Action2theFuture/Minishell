/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memmove.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: junsan <junsan@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/08 17:02:16 by junsan            #+#    #+#             */
/*   Updated: 2023/11/14 17:45:43 by junsan           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_memmove(void *dest, const void *src, size_t n)
{
	unsigned char	*tmp_dest;
	unsigned char	*tmp_src;
	size_t			i;

	if ((!dest && !src))
		return (0);
	tmp_dest = (unsigned char *)dest;
	tmp_src = (unsigned char *)src;
	i = 0;
	if (dest > src && n > 0)
	{
		while (n--)
		{
			tmp_dest[n] = tmp_src[n];
		}
	}
	else
	{
		while (n--)
		{
			tmp_dest[i] = tmp_src[i];
			i++;
		}
	}
	return (tmp_dest);
}
/*
#include <stdio.h>
#include <string.h>

char	message2[60] = "abcdefghijklmnopqrstuvwxyz";
char	temp[60];

int	main(int argc, char const *argv[])
{
	strcpy(temp, message2);
	printf("\n\nOriginal message: %s", temp);
	memcpy(temp + 4, temp + 16, 10);
	printf("\nAfter memcpy() without overlap:\t%s", temp);
	strcpy(temp, message2);
	memcpy(temp + 6, temp + 4, 10);
	printf("\nAfter memcpy() with overlap:\t%s", temp);
	strcpy(temp, message2);
	printf("\n\nOriginal message: %s", temp);
	ft_memmove(temp + 4, temp + 16, 10);
	printf("\nAfter memmove() without overlap:\t%s", temp);
	strcpy(temp, message2);
	ft_memmove(temp + 6, temp + 4, 10);
	printf("\nAfter memmove() with overlap:\t%s\n", temp);
	return (0);
}
*/
