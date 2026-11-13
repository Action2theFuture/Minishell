/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memchr.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: junsan <junsan@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/09 16:17:53 by junsan            #+#    #+#             */
/*   Updated: 2023/11/13 11:35:42 by junsan           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_memchr(const void *ptr, int value, size_t num)
{
	size_t	i;

	i = 0;
	while (num--)
	{
		if (((unsigned char *)ptr)[i] == (unsigned char)value)
			return (&((char *)ptr)[i]);
		i++;
	}
	return (NULL);
}
/*#include <stdio.h>

int	main(void) {
  char* pch;
  char str[] = "Example string";
  pch = (char*)ft_memchr(str, 'p', ft_strlen(str));

  if (pch != NULL)
	printf("'p' found at position %ld.\n", pch - str + 1);
  else
	printf("'p' not found.\n");

  return (0);
}*/
