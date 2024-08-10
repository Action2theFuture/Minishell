/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing_utils_2.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: junsan <junsan@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/27 15:40:23 by junsan            #+#    #+#             */
/*   Updated: 2024/08/08 09:42:24 by junsan           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	add_parent(t_ast *node, t_ast *left, t_ast *right)
{
	if (left)
		left->parent = node;
	if (right)
		right->parent = node;
}

char	*append_with_semicolon(char *original, char *to_append)
{
	size_t	original_len;
	size_t	to_append_len;
	size_t	new_len;
	char	*new_str;

	if (original == NULL || to_append == NULL)
		return (NULL);
	original_len = ft_strlen(original);
	to_append_len = ft_strlen(to_append);
	new_len = original_len + to_append_len + 2;
	new_str = (char *)malloc(sizeof(char) * new_len);
	if (new_str == NULL)
	{
		perror("malloc");
		return (NULL);
	}
	ft_strlcpy(new_str, original, new_len);
	new_str[original_len] = ARR_SEP;
	new_str[original_len + 1] = '\0';
	ft_strlcpy(\
			new_str + original_len + 1, to_append, new_len - original_len - 1);
	free((char *)original);
	return (new_str);
}
