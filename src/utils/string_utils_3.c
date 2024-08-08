/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   string_utils_3.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: junsan <junsan@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/08 09:42:51 by junsan            #+#    #+#             */
/*   Updated: 2024/08/08 09:43:00 by junsan           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*concat_three_strings(char *str1, char *str2, char *str3)
{
	size_t	new_data_len;
	char	*new_data;

	new_data_len = ft_strlen(str1) + ft_strlen(str2) + 2;
	new_data = (char *)malloc(sizeof(char) * (new_data_len + 1));
	if (!new_data)
	{
		perror("malloc error");
		return (NULL);
	}
	ft_strlcpy(new_data, str1, new_data_len);
	ft_strlcat(new_data, str2, new_data_len);
	ft_strlcat(new_data, str3, new_data_len);
	return (new_data);
}
