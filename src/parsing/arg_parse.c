/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   arg_parse.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: junsan <junsan@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/04 14:14:41 by junsan            #+#    #+#             */
/*   Updated: 2024/08/03 14:11:24 by junsan           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static size_t	get_total_arg_list_size(t_token *token)
{
	t_token	*cur;
	t_token	*prev;
	size_t	total_len;

	cur = token;
	prev = NULL;
	total_len = 0;
	while (cur)
	{
		if (cur->type != CMD)
			break ;
		if (prev)
			total_len += ft_strlen(prev->data) + 1;
		prev = cur;
		cur = cur->next;
	}
	if (prev)
		total_len += ft_strlen(prev->data);
	return (total_len);
}

static void	file_data(char *data, t_token **token)
{
	char	*ptr;
	int		len;

	ptr = data;
	while (*token && (*token)->type == CMD)
	{
		len = ft_strlen((*token)->data);
		if (len > 0 && \
			!(len == 2 && (*token)->data[0] == '"' && (*token)->data[1] == '"'))
		{
			ft_strlcpy(ptr, (*token)->data, len + 1);
			ptr += ft_strlen((*token)->data);
			if ((*token)->next && (*token)->next->type == CMD)
			{
				*ptr = ARR_SEP;
				ptr++;
			}
		}
		*token = (*token)->next;
	}
	*ptr = '\0';
}

char	*arg_parsing(t_token **token)
{
	size_t	total_len;
	char	*data;

	total_len = get_total_arg_list_size(*token);
	data = (char *)malloc(sizeof(char) * total_len + 1);
	if (!data)
		return (perror("malloc error"), NULL);
	file_data(data, token);
	return (data);
}
