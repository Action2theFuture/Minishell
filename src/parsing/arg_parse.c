/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   arg_parse.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: junsan <junsan@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/29 18:50:55 by junsan            #+#    #+#             */
/*   Updated: 2024/06/27 09:56:29 by junsan           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static size_t	arg_len(const char *arg)
{
	size_t	len;

	len = ft_strlen(arg);
	if (arg[0] == '-' && ft_strlen(arg) > 2)
		return ((len - 1 * 2) + len - 2);
	return (len);
}

static size_t	get_total_arg_list_size(t_token **token)
{
	t_token	*cur;
	t_token	*prev;
	size_t	total_len;

	cur = *token;
	prev = NULL;
	total_len = 0;
	while (cur)
	{
		if (cur->type != CMD)
			break ;
		if (prev)
			total_len += arg_len(prev->data) + 1;
		prev = cur;
		cur = cur->next;
	}
	if (prev)
		total_len += arg_len(prev->data);
	*token = cur;
	return (total_len);
}

static void	split_flag(char *dest, const char *src)
{
	size_t	i;

	i = 1;
	while (i < ft_strlen(src))
	{
		*dest = '-';
		dest++;
		*dest = src[i];
		dest++;
		if (i < ft_strlen(src) - 1)
		{
			*dest = ARR_SEP;
			dest++;
		}
		i++;
	}
	*dest = '\0';
}

static void	file_data(char *data, t_token **token)
{
	char	*ptr;

	ptr = data;
	while (*token && (*token)->type == CMD)
	{
		// remove_double_quotes((*token)->data);
		if ((*token)->data[0] == '-' && ft_strlen((*token)->data) > 2)
		{
			split_flag(ptr, (*token)->data);
			ptr += (ft_strlen((*token)->data) - 1) * 2 \
			+ (ft_strlen((*token)->data) - 2);
		}
		else
		{
			ft_strlcpy(ptr, (*token)->data, ft_strlen((*token)->data) + 1);
			ptr += ft_strlen((*token)->data);
		}
		if ((*token)->next && (*token)->next->type == CMD)
		{
			*ptr = ARR_SEP;
			ptr++;
		}
		*token = (*token)->next;
	}
	*ptr = '\0';
}

char	*arg_parsing(t_token **token)
{
	t_token	*cur;
	size_t	total_len;
	char	*data;

	cur = *token;
	// printf("token data : %s\n", cur->data);
	total_len = get_total_arg_list_size(&cur);
	data = (char *)malloc(sizeof(char) * (total_len + 1));
	if (!data)
		return (NULL);
	file_data(data, token);
	return (data);
}
