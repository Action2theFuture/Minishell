/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handler_operators_and_spaces.c                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: junsan <junsan@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/13 19:46:03 by junsan            #+#    #+#             */
/*   Updated: 2024/08/06 16:13:35 by junsan           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	handle_repeated_operators(\
				const char **input, const char **start, t_token **list)
{
	int	cnt;

	cnt = count_repeated_chars(*input, **input);
	if (cnt < 3)
	{
		if (cnt > 1 && *((*input) + 1) == **input)
			(add_token(list, *input, 2), *input += 2);
		else
			(add_token(list, *input, 1), *input += 1);
	}
	else if (cnt == 3 && ft_strncmp(*input, "<<<", 3) == 0)
		(add_token(list, *input, 3), *input += 3);
	else
		(add_token(list, *input, cnt), *input += cnt);
	*start = *input;
}

static char	*concat_strings(const char *str1, const char *str2)
{
	size_t	len1;
	size_t	len2;
	size_t	new_len;
	char	*new_data;

	len1 = ft_strlen(str1);
	len2 = ft_strlen(str2);
	new_len = len1 + len2 + 1;
	new_data = (char *)malloc(sizeof(char) * new_len);
	if (new_data)
	{
		ft_strlcpy(new_data, str1, new_len);
		ft_strlcat(new_data, str2, new_len);
	}
	return (new_data);
}

static void	merge_last_two_tokens(t_token **head)
{
	t_token	*cur;
	t_token	*prev;
	char	*new_data;

	if (!*head || !(*head)->next)
		return ;
	cur = *head;
	prev = NULL;
	while (cur->next)
	{
		prev = cur;
		cur = cur->next;
	}
	if (prev && cur)
	{
		new_data = concat_strings(prev->data, cur->data);
		if (new_data)
			(free(prev->data), prev->data = new_data);
	}
	if (prev)
		prev->next = NULL;
	(free(cur->data), free(cur));
}

// SHELL_OPERATORS "()&|<>"
// The reason for hand;e quotes is because examples like abc"defg"abc
static void	handle_cmd(const char **input, const char **start, t_token **list)
{
	while (**input && \
	!ft_strchr(SHELL_METACHARS, **input) && !ft_isspace(**input))
		(*input)++;
	if (*input > *start)
	{
		add_token(list, *start, *input - *start);
		if (**input == '\'' || **input == '"')
		{
			handle_quotes(input, start, list);
			merge_last_two_tokens(list);
		}
	}
	*start = *input;
}

// DELIMS "|&<>"
void	handle_operators_and_spaces(\
	const char **input, const char **start, t_token **list)
{
	const char	*delims;

	delims = DELIMS;
	while (**input && ft_isspace(**input))
		(*input)++;
	*start = *input;
	if (ft_strchr(delims, **input))
		handle_repeated_operators(input, start, list);
	else
		handle_cmd(input, start, list);
}
