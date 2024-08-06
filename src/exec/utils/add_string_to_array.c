/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_cmd_line_with_quotes.c                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: junsan <junsan@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/29 13:19:33 by junsan            #+#    #+#             */
/*   Updated: 2024/08/06 20:06:58 by junsan           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	**add_string_to_array(char **array, int *size, const char *str)
{
	char	**new_array;
	int		i;

	new_array = (char **)malloc(sizeof(char *) * (*size + 2));
	if (new_array == NULL)
		return (perror("malloc error"), NULL);
	i = -1;
	while (++i < *size)
		new_array[i] = array[i];
	new_array[*size] = ft_strdup(str);
	if (new_array[*size] == NULL)
	{
		(perror("strdup error"), free(new_array));
		return (NULL);
	}
	new_array[*size + 1] = NULL;
	free(array);
	(*size)++;
	return (new_array);
}
