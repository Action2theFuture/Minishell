/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memset.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: junsan <junsan@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/08 17:00:18 by junsan            #+#    #+#             */
/*   Updated: 2023/11/13 11:42:47 by junsan           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_memset(void *ptr, int value, size_t num)
{
	unsigned char	*tmp;
	size_t			i;

	tmp = ptr;
	i = 0;
	while (i < num)
	{
		tmp[i] = value;
		i++;
	}
	return (ptr);
}
/*
#include <stdio.h>

int	main(void) {
  char str[] = "almost every programmer should know memset!";
  ft_memset(str, '-', 6);
  puts(str);
  return (0);
}*/
