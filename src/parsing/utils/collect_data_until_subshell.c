/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   collect_data_until_subshell.c                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: junsan <junsan@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/28 11:20:27 by junsan            #+#    #+#             */
/*   Updated: 2024/07/29 18:28:04 by junsan           ###   ########.fr       */
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

static void	append_token_data(\
char **data_in_subshell, size_t *total_len, size_t *capacity, t_token **token)
{
	size_t	data_len;
	size_t	space_needed;

	data_len = ft_strlen((*token)->data);
	space_needed = *total_len + data_len + 1;
	if ((*token)->next && (*token)->next->type != SUBSHELL)
		space_needed++;
	ensure_capacity(data_in_subshell, capacity, space_needed);
	if (*data_in_subshell == NULL)
		return ;
	ft_strlcat(*data_in_subshell, (*token)->data, *capacity);
	*total_len += data_len;
	if ((*token)->next && (*token)->next->type != SUBSHELL)
	{
		ft_strlcat(*data_in_subshell, " ", *capacity);
		(*total_len)++;
	}
}

static bool	handle_subshell_cmd_only(\
t_token **token, char **data_in_subshell, size_t *total_len, size_t *capacity)
{
	t_token	*token_in_subshell;

	token_in_subshell = (*token)->next;
	while (token_in_subshell && token_in_subshell->type != SUBSHELL)
	{
		if (token_in_subshell->type == LOGICAL)
			return (false);
		token_in_subshell = token_in_subshell->next;
	}
	(*token) = (*token)->next;
	while (*token && (*token)->type != SUBSHELL)
	{
		append_token_data(data_in_subshell, total_len, capacity, token);
		if (*data_in_subshell == NULL)
			return (false);
		*token = (*token)->next;
	}
	return (true);
}

static char	*append_data_until_subshell(\
size_t *total_len, size_t *capacity, char *data_in_subshell, t_token **token)
{
	t_token	*check_token;
	int		logical_cnt;

	check_token = NULL;
	logical_cnt = 0;
	while (*token && (*token)->type != SUBSHELL && logical_cnt < 2)
	{
		append_token_data(&data_in_subshell, total_len, capacity, token);
		if (data_in_subshell == NULL)
			return (NULL);
		check_token = *token;
		*token = (*token)->next;
		if (*token && (*token)->type == LOGICAL)
			logical_cnt++;
	}
	if (*token && (*token)->data[0] == '(' && \
		check_token && is_logical_operator(check_token->data))
	{
		if (!handle_subshell_cmd_only(\
				token, &data_in_subshell, total_len, capacity))
			return (NULL);
	}
	return (data_in_subshell);
}

char	*collect_data_until_subshell(t_token **token)
{
	size_t	capacity;
	size_t	total_len;
	char	*data_in_subshell;

	total_len = 0;
	capacity = MEMORY_CAPACITY;
	data_in_subshell = (char *)malloc(sizeof(char) * capacity);
	if (!data_in_subshell)
		return (NULL);
	data_in_subshell[0] = '\0';
	return (append_data_until_subshell(\
				&total_len, &capacity, data_in_subshell, token));
}
