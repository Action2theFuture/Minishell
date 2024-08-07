/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   add_string_to_array.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: junsan <junsan@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/29 13:19:33 by junsan            #+#    #+#             */
/*   Updated: 2024/08/07 12:29:14 by junsan           ###   ########.fr       */
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

static bool	handle_quotes_in_str(\
			const char **ptr, bool *in_single_quotes, bool *in_double_quotes)
{
	if (**ptr == '\'' && !(*in_double_quotes))
	{
		*in_single_quotes = !(*in_single_quotes);
		(*ptr)++;
		return (true);
	}
	else if (**ptr == '"' && !(*in_single_quotes))
	{
		*in_double_quotes = !(*in_double_quotes);
		(*ptr)++;
		return (true);
	}
	return (false);
}

char	*remove_shell_quotes(const char *str)
{
	const char	*ptr;
	char		*new_str;
	char		*new_str_ptr;
	bool		in_single_quotes;
	bool		in_double_quotes;

	new_str = (char *)malloc(sizeof(char) * (ft_strlen(str) + 1));
	if (!new_str)
		return (perror("malloc error"), NULL);
	in_single_quotes = false;
	in_double_quotes = false;
	ptr = str;
	new_str_ptr = new_str;
	while (*ptr)
	{
		if (handle_quotes_in_str(&ptr, &in_single_quotes, &in_double_quotes))
			continue ;
		*new_str_ptr++ = *ptr++;
	}
	*new_str_ptr = '\0';
	return (new_str);
}
