/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   collect_data_until_subshell.c                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: junsan <junsan@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/28 11:20:27 by junsan            #+#    #+#             */
/*   Updated: 2024/08/19 22:15:01 by junsan           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	append_token_data(\
char **data_in_subshell, size_t *total_len_and_capacity, t_token **token)
{
	size_t	data_len;
	size_t	space_needed;

	data_len = ft_strlen((*token)->data);
	space_needed = total_len_and_capacity[0] + data_len + 1;
	if ((*token)->next && (*token)->next->type != SUBSHELL)
		space_needed++;
	ensure_capacity(data_in_subshell, &total_len_and_capacity[1], space_needed);
	if (*data_in_subshell == NULL)
		return ;
	ft_strlcat(*data_in_subshell, (*token)->data, total_len_and_capacity[1]);
	total_len_and_capacity[0] += data_len;
	if ((*token)->next && (*token)->next->type != SUBSHELL)
	{
		ft_strlcat(*data_in_subshell, " ", total_len_and_capacity[1]);
		total_len_and_capacity[0]++;
	}
}

static char	*append_data_until_subshell(\
size_t *total_len_and_capacity, char *data_in_subshell, \
t_token **token, bool *is_pipe_or_logical)
{
	bool	have_logical;
	bool	have_pipe;

	have_pipe = is_pipe_or_logical[0];
	have_logical = is_pipe_or_logical[1];
	while (*token)
	{
		append_token_data(&data_in_subshell, total_len_and_capacity, token);
		if (data_in_subshell == NULL)
			return (NULL);
		*token = (*token)->next;
		if ((have_logical || have_pipe) && (*token) && \
			((*token)->type == LOGICAL || (*token)->type == PIPE))
		{
			if ((*token)->next && (*token)->next->type == SUBSHELL)
				break ;
		}
		if (*token && (*token)->type == LOGICAL)
			have_logical = true;
		if (*token && (*token)->type == PIPE)
			have_pipe = true;
		if (*token && (*token)->type == SUBSHELL)
			break ;
	}
	return (data_in_subshell);
}

static void	append_token_data_for_nested(\
char **data_in_subshell, size_t *total_len_and_capacity, t_token **token)
{
	size_t	data_len;
	size_t	space_needed;

	data_len = ft_strlen((*token)->data);
	space_needed = total_len_and_capacity[0] + data_len + 1;
	if ((*token)->next)
		space_needed++;
	ensure_capacity(data_in_subshell, &total_len_and_capacity[1], space_needed);
	if (*data_in_subshell == NULL)
		return ;
	ft_strlcat(*data_in_subshell, (*token)->data, total_len_and_capacity[1]);
	total_len_and_capacity[0] += data_len;
}

static char	*append_data_until_nested_subshell(\
size_t *total_len_and_capacity, char *data_in_subshell, t_token **token)
{
	int	depth;

	depth = 0;
	while (*token)
	{
		append_token_data_for_nested(\
						&data_in_subshell, total_len_and_capacity, token);
		if (data_in_subshell == NULL)
			return (NULL);
		if ((*token)->type == SUBSHELL && (*token)->data[0] == '(')
			depth++;
		else if ((*token)->type == SUBSHELL && (*token)->data[0] == ')')
			depth--;
		*token = (*token)->next;
		if (depth == 0)
			break ;
		ft_strlcat(data_in_subshell, " ", total_len_and_capacity[1]);
		total_len_and_capacity[0]++;
	}
	return (data_in_subshell);
}


// total_len_and_capacity 
// -> first ele is total_len, second ele is capacity
char	*collect_data_until_subshell(\
		t_token **token, int subshell_status, bool *is_pipe_or_logical)
{
	size_t	total_len_and_capacity[2];
	char	*data_in_subshell;

	total_len_and_capacity[0] = 0;
	total_len_and_capacity[1] = MEMORY_CAPACITY;
	data_in_subshell = (char *)malloc(sizeof(char) * total_len_and_capacity[1]);
	if (!data_in_subshell)
		return (NULL);
	data_in_subshell[0] = '\0';
	if (subshell_status == NESTED && (*token)->type == SUBSHELL)
		return (append_data_until_nested_subshell(\
						total_len_and_capacity, data_in_subshell, token));
	return (append_data_until_subshell(\
			total_len_and_capacity, data_in_subshell, token, is_pipe_or_logical));
}
