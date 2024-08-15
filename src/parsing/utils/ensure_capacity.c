/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ensure_capacity.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: junsan <junsan@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/10 14:16:43 by junsan            #+#    #+#             */
/*   Updated: 2024/08/10 14:16:51 by junsan           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	ensure_capacity(\
	char **data_in_subshell, size_t *capacity, size_t space_needed)
{
	char	*new_data;

	while (space_needed > *capacity)
	{
		*capacity *= 2;
		new_data = (char *)malloc(sizeof(char) * (*capacity));
		if (!new_data)
		{
			free(*data_in_subshell);
			*data_in_subshell = NULL;
			perror("malloc error");
			return ;
		}
		ft_strlcpy(new_data, *data_in_subshell, *capacity);
		free(*data_in_subshell);
		*data_in_subshell = new_data;
	}
}
