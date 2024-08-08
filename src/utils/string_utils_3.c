/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   string_utils_3.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: junsan <junsan@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/08 09:42:51 by junsan            #+#    #+#             */
/*   Updated: 2024/08/08 10:28:18 by junsan           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*concat_three_strings(char *str1, char *str2, char *str3)
{
	size_t	new_str_len;
	char	*new_str;

	new_str_len = ft_strlen(str1) + ft_strlen(str2) + ft_strlen(str3) + 1;
	new_str = (char *)malloc(sizeof(char) * (new_str_len + 1));
	if (!new_str)
		return (perror("malloc error"), NULL);
	ft_strlcpy(new_str, str1, new_str_len);
	ft_strlcat(new_str, str2, new_str_len);
	ft_strlcat(new_str, str3, new_str_len);
	return (new_str);
}

char	*concat_two_strings(char *str1, char *str2)
{
	size_t	new_str_len;
	char	*new_str;

	new_str_len = ft_strlen(str1) + ft_strlen(str2) + 1;
	new_str = (char *)malloc(sizeof(char) * new_str_len);
	if (!new_str)
		return (perror("malloc error"), NULL);
	ft_strlcpy(new_str, str1, new_str_len);
	ft_strlcat(new_str, str2, new_str_len);
	return (new_str);
}
