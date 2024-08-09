/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   collect_data_until_subshell.c                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: junsan <junsan@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/28 11:20:27 by junsan            #+#    #+#             */
/*   Updated: 2024/08/10 13:05:46 by junsan           ###   ########.fr       */
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

static char	*append_data_until_subshell(\
size_t *total_len, size_t *capacity, char *data_in_subshell, t_token **token)
{
	bool	have_logical;
	bool	have_pipe;

	have_logical = false;
	have_pipe = false;
	while (*token)
	{
		append_token_data(&data_in_subshell, total_len, capacity, token);
		if (data_in_subshell == NULL)
			return (NULL);
		*token = (*token)->next;
		if ((have_logical && (*token) && (*token)->type == LOGICAL) || \
			(have_pipe && (*token) && (*token)->type == PIPE))
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
char **data_in_subshell, size_t *total_len, size_t *capacity, t_token **token)
{
	size_t	data_len;
	size_t	space_needed;

	data_len = ft_strlen((*token)->data);
	space_needed = *total_len + data_len + 1;
	if ((*token)->next)
		space_needed++;
	ensure_capacity(data_in_subshell, capacity, space_needed);
	if (*data_in_subshell == NULL)
		return ;
	ft_strlcat(*data_in_subshell, (*token)->data, *capacity);
	*total_len += data_len;
	if ((*token)->next)
	{
		ft_strlcat(*data_in_subshell, " ", *capacity);
		(*total_len)++;
	}
}

static char	*append_data_until_nested_subshell(\
size_t *total_len, size_t *capacity, char *data_in_subshell, t_token **token)
{
	int	depth;

	depth = 0;
	while (*token)
	{
		append_token_data_for_nested(&data_in_subshell, total_len, capacity, token);
		if (data_in_subshell == NULL)
			return (NULL);
		if ((*token)->type == SUBSHELL && (*token)->data[0] == '(')
			depth++;
		else if ((*token)->type == SUBSHELL && (*token)->data[0] == ')')
			depth--;
		if (depth == 0)
			break ;
		*token = (*token)->next;
	}
	return (data_in_subshell);
}

char	*collect_data_until_subshell(t_token **token, int subshell_status)
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
	if (subshell_status == NESTED)
		return (append_data_until_nested_subshell(\
						&total_len, &capacity, data_in_subshell, token));
	return (append_data_until_subshell(\
				&total_len, &capacity, data_in_subshell, token));
}
