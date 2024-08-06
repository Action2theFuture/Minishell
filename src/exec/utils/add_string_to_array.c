/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   add_string_to_array.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: junsan <junsan@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/29 13:19:33 by junsan            #+#    #+#             */
/*   Updated: 2024/08/06 21:10:02 by junsan           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	**add_string_to_array(int *size, const char *str)
{
	char	**new_array;

	new_array = (char **)malloc(sizeof(char *) * (*size + 2));
	if (new_array == NULL)
		return (perror("malloc error"), NULL);
	new_array[*size] = ft_strdup(str);
	if (new_array[*size] == NULL)
	{
		(perror("strdup error"), free(new_array));
		return (NULL);
	}
	new_array[*size + 1] = NULL;
	(*size)++;
	return (new_array);
}
